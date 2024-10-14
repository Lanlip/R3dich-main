import { SecureSMSAuthenticationRequest, SecureSMSAuthenticationStatus } from "../types.js";
export declare class SecureSMSAuthenticator {
    uuid: string;
    private _cookie;
    private userAgent;
    private browserUserAgent;
    private _authenticationRequest;
    private _verified;
    private _details;
    constructor(uuid?: string);
    initialize(): Promise<SecureSMSAuthenticationRequest>;
    check(): Promise<SecureSMSAuthenticationStatus>;
    get agents(): {
        userAgent: string;
        browserUserAgent: string;
    };
    get details(): {
        verified: boolean;
        phone: string;
    };
    get verified(): boolean;
    get authenticationRequest(): SecureSMSAuthenticationRequest | null;
    get cookie(): string | null;
}
