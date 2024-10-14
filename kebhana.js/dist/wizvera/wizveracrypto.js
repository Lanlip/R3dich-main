import { createCipheriv, createDecipheriv, createHash, randomBytes } from "crypto";
export class WizveraCrypto {
    input;
    key;
    constructor(input) {
        this.input = input;
        this.key = createHash("sha256").update(`WIZVERA ID2` + input).digest();
        return this;
    }
    static sha256(data, repeat = 1) {
        let hash = data;
        for (let i = 0; i < repeat; i++) {
            hash = createHash("sha256").update(hash).digest();
        }
        return hash;
    }
    encrypt(data) {
        const iv = randomBytes(16);
        const cipher = createCipheriv("aes-256-cbc", this.computeKey(iv), iv);
        return Buffer.concat([iv, cipher.update(data), cipher.final()]);
    }
    decrypt(_data) {
        const iv = Buffer.alloc(16);
        _data.copy(iv, 0, 0, 16);
        const key = this.computeKey(iv);
        const data = Buffer.alloc(_data.length - 16);
        _data.copy(data, 0, 16);
        const decipher = createDecipheriv("aes-256-cbc", key, iv);
        return Buffer.concat([decipher.update(data), decipher.final()]);
    }
    computeKey(iv) {
        return createHash("sha256").update(Buffer.concat([this.key, Buffer.from([0, 0, 0, 1]), iv])).digest();
    }
}
;
