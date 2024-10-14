import { createCipheriv, createDecipheriv, createHash, randomBytes } from "crypto";

export class WizveraCrypto {
    private key: Buffer;

    constructor (public input: string) {
        this.key = createHash("sha256").update(`WIZVERA ID2` + input).digest();
        return this;
    }

    static sha256(data: Buffer, repeat: number = 1): Buffer {
        let hash = data;
        for (let i = 0; i < repeat; i++) {
            hash = createHash("sha256").update(hash).digest();
        }
        return hash;
    }

    public encrypt(data: Buffer): Buffer {
        const iv = randomBytes(16);
        const cipher = createCipheriv("aes-256-cbc", this.computeKey(iv), iv);
        return Buffer.concat([iv, cipher.update(data), cipher.final()]);
    }

    public decrypt(_data: Buffer): Buffer {
        const iv = Buffer.alloc(16);
        _data.copy(iv, 0, 0, 16);
        const key = this.computeKey(iv);
        const data = Buffer.alloc(_data.length - 16);
        _data.copy(data, 0, 16);
        const decipher = createDecipheriv("aes-256-cbc", key, iv);
        return Buffer.concat([decipher.update(data), decipher.final()]);
    }

    public computeKey(iv: Buffer): Buffer {
        return createHash("sha256").update(Buffer.concat([this.key, Buffer.from([0, 0, 0, 1]), iv])).digest();
    }
};