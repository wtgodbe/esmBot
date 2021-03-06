#include <napi.h>
#include <list>
#include <Magick++.h>

using namespace std;
using namespace Magick;

class WdtWorker : public Napi::AsyncWorker {
 public:
  WdtWorker(Napi::Function& callback, string in_path, string type, int delay)
      : Napi::AsyncWorker(callback), in_path(in_path), type(type), delay(delay) {}
  ~WdtWorker() {}

  void Execute() {
    list <Image> frames;
    list <Image> coalesced;
    list <Image> mid;
    list <Image> result;
    Image watermark;
    readImages(&frames, in_path);
    watermark.read("./assets/images/whodidthis.png");
    coalesceImages(&coalesced, frames.begin(), frames.end());

    for (Image &image : coalesced) {
      Image watermark_new = watermark;
      image.scale(Geometry("374x374>"));
      watermark_new.composite(image, Magick::CenterGravity, Magick::OverCompositeOp);
      watermark_new.magick(type);
      mid.push_back(watermark_new);
    }

    optimizeImageLayers(&result, mid.begin(), mid.end());
    if (delay != 0) for_each(result.begin(), result.end(), animationDelayImage(delay));
    writeImages(result.begin(), result.end(), &blob);
  }

  void OnOK() {
    Callback().Call({Env().Undefined(), Napi::Buffer<char>::Copy(Env(), (char *)blob.data(), blob.length())});
  }

 private:
  string in_path, type;
  int delay;
  Blob blob;
};

Napi::Value Wdt(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  Napi::Object obj = info[0].As<Napi::Object>();
  Napi::Function cb = info[1].As<Napi::Function>();
  string path = obj.Get("path").As<Napi::String>().Utf8Value();
  string type = obj.Get("type").As<Napi::String>().Utf8Value();
  int delay = obj.Get("delay").As<Napi::Number>().Int32Value();

  WdtWorker* blurWorker = new WdtWorker(cb, path, type, delay);
  blurWorker->Queue();
  return env.Undefined();
}