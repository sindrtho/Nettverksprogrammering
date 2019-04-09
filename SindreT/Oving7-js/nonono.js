var crypto  = require('crypto');

const key = "-----BEGIN RSA PUBLIC KEY-----\n" +
    "MIIBCgKCAQEA639u2haGdEoEQ5wf7lfTHEvDW2FuLBNmZgailV3N9L2JCI9NKtk1\n" +
    "QOlEW2t6jweRfzjNf7Qs9XZkk6v6hveW2AZAYuhbNxQFT1FOk+Ez2RFVLLNZfIc+\n" +
    "sXD0VURkORY7m+CFHfT+pf6hlLrvZONEWdJ1ZmxDtMOH6hTESCOooxdJ8m2+WsA5\n" +
    "GuzOvaagZD/P4Gf9uoVjk/+G4jsB3YyaGAu+hs/Xx/ti9xPwFtCiUloJlUxhsDz9\n" +
    "my67QMmPype4vv1w2Hhaj3UabCQi5qj4JgSctNayRy73Wk0iXtos1s2S38CUsUuS\n" +
    "L7oZWDeIi2pZS0NT7e8cZllAHgSuX8MW+wIDAQAB\n" +
    "-----END RSA PUBLIC KEY-----";

const signature = "9c3e8d77333fcee3885747250fd48c8a6a5a8e62c24f8ef5f578c752469880409f69fa94a70dae0f71acc7a3988cc81e66881cbc75d5096dedfeeb3d17fb88fd27abe5d32f3b705a11045a91b5b5986f34948009e9b35e8026f986ae871e986392ae37e0458223d62b05fbb50935f63fa920590454d7851d35bf7b3d4cf0752c4683666bcb0398843d141113f32442f8d38f7910a43102da331a6e56fd2a3b3dbe49abf15b4e93c5a81341ed9f87e6bd972536e185e2cde096105db51de519f980901585b2c312b8a097853434bf144a3f14182f2d1b971169280b15061b781a21b8954c626aa4d9417275c1b1812eb0b9770b8320db2f1093f6e775105d39d5"
const message = "important message!";

let sigbuf = Buffer.from(signature, 'hex');
let dec_sig = crypto.publicDecrypt(key, sigbuf);

let hashed_message = "30781fa28aacb1785eb363ab9f61722de60df337"; // Made in python script.
let hash_buf = Buffer.from(hashed_message, 'hex');

console.log(hash_buf);
console.log(dec_sig);

// Mesage is not tampered with!