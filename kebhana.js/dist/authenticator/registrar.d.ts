/// <reference types="node" resolution-mode="require"/>
export declare class KebHanaPINRegistrar {
    private pin;
    private _cookie;
    private uuid;
    private pinSignURL;
    private userAgent;
    private browserUserAgent;
    private bin;
    private secret;
    private cryptoKey;
    private crypto1;
    private crypto2;
    private _ecPublic;
    private _ecPrivate;
    private _jwk;
    constructor(pin: string, _cookie: string, uuid?: string);
    register(): Promise<{
        berry: any;
        privateKey: string;
        custNo: any;
        uuid: any;
    }>;
    get ecPublic(): Buffer;
    get ecPrivate(): string;
    get jwk(): {
        kty: string;
        crv: string;
        x: string;
        y: string;
    };
}
