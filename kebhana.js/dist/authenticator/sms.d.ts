import { SMSAuthenticatorConfig } from "../types.js";
import { SecureSMSAuthenticator } from "./securesms.js";
import { KebHanaPINRegistrar } from "./registrar.js";
export declare class SMSAuthenticator {
    private _secureAuthenticator;
    private _sent;
    private _config;
    private _verified;
    constructor(_secureAuthenticator: SecureSMSAuthenticator);
    requestSMS(config: SMSAuthenticatorConfig): Promise<void>;
    verify(code: string): Promise<void>;
    createPINRegistrar(pin: string): KebHanaPINRegistrar;
    get sent(): boolean;
    get verified(): boolean;
}
