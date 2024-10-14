/// <reference types="node" resolution-mode="require"/>
export declare class WizveraCrypto {
    input: string;
    private key;
    constructor(input: string);
    static sha256(data: Buffer, repeat?: number): Buffer;
    encrypt(data: Buffer): Buffer;
    decrypt(_data: Buffer): Buffer;
    computeKey(iv: Buffer): Buffer;
}
