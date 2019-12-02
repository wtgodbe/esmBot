const client = require("../utils/client.js");
const moment = require("moment");
require("moment-duration-format");
const os = require("os");

exports.run = async (message) => {
  const duration = moment.duration(client.uptime).format(" D [days], H [hrs], m [mins], s [secs]");
  const embed = {
    embed: {
      "author": {
        "name": "esmBot Statistics",
        "icon_url": client.user.avatarURL
      },
      "color": 16711680,
      "description": "**You are currently using esmBot Dev! Things may change at any time without warning and there will be bugs. Many bugs.**",
      "fields": [{
        "name": "Memory Usage",
        "value": `${(process.memoryUsage().heapUsed / 1024 / 1024).toFixed(2)} MB`
      },
      {
        "name": "Uptime",
        "value": duration
      },
      {
        "name": "Host",
        "value": `${os.type()} ${os.release()} (${os.arch()})`
      },
      {
        "name": "Library",
        "value": `Eris ${require("eris").VERSION}`
      },
      {
        "name": "Node.js Version",
        "value": process.version
      }
      ]
    }
  };
  return message.channel.createMessage(embed);
};

exports.aliases = ["status", "stat"];
exports.category = 1;
exports.help = "Gets some statistics about me";