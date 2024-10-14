import axios from "axios";
import { SMSAuthenticatorConfig } from "../types.js";
import { SecureSMSAuthenticator } from "./securesms.js";
import { KebHanaPINRegistrar } from "./registrar.js";

export class SMSAuthenticator {
    private _sent: boolean = false;
    private _config: SMSAuthenticatorConfig | null = null;
    private _verified: boolean = false;

    constructor(private _secureAuthenticator: SecureSMSAuthenticator) {
        if (!_secureAuthenticator) {
            throw new Error("SecureSMSAuthenticator instance is required.");
        }

        if (!_secureAuthenticator.authenticationRequest) {
            throw new Error("SecureSMSAuthenticator instance must have authentication request.");
        }

        if (!_secureAuthenticator.verified) {
            throw new Error("SecureSMSAuthenticator instance must be verified.");
        }
    }

    async requestSMS(config: SMSAuthenticatorConfig) {
        const response = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM001200501_P02.do", {
            certCustNm: "",
            certExpiryDate: "",
            isTab: "N",
            tab: "1",
            certStateYn: "N",
            fullMbphNo: this._secureAuthenticator.details.phone.substring(0, 3),
            telecom: config.telecom,
            custNm: config.name,
            rsbzRegNoFront: config.ssnFront,
            rsbzRegNoBack: config.ssnBack,
            mbphNoFront: this._secureAuthenticator.details.phone.substring(0, 3),
            mbphNoBack: this._secureAuthenticator.details.phone.substring(3),
            uuid: this._secureAuthenticator.uuid,
            SERVER_DOMAIN: "https://mbp.hanabank.com",
            mainPage: "Y",
            hanaHistoryType: "2",
            이전메뉴ID: "CCOM0012004_P01",
            custRegPage: "Y",
            certTypDvCd: "",
            nextPage: "http://127.0.0.1:14501/mbp/resource/html/CREG/CREG01/CREG0101009.html",
            isPushHistoryNext: "false",
            type: "rsbzRegNo7",
            custCertNo: "",
            unqNo: "",
            viewTossAuth: "Y",
            mbphEnprDvCd: "",
            mbphNo: this._secureAuthenticator.details.phone,
            completeYn: "Y",
            rspsRsltCd: "M000",
            noFace: "Y",
            nfilterFields: "rsbzRegNoBack=num"
        }, {
            headers: {
                "User-Agent": this._secureAuthenticator.agents.browserUserAgent,
                "CUSTOM_USER_AGENT": this._secureAuthenticator.agents.userAgent,
                "Content-Type": "application/x-www-form-urlencoded",
                "Cookie": `JSESSIONID=${this._secureAuthenticator.cookie};`,
            }
        });

        if (response.data.headerData.status !== 200) throw new Error(`Failed to request SMS authentication: ${response.data.headerData.errorMessage} (${response.data.headerData.errorCode})`);

        const smsResponse = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM001200502_P02.do", {
            mbphEnprDvCd: config.telecom,
            mbphNo: this._secureAuthenticator.details.phone,
            custNm: config.name,
        }, {
            headers: {
                "User-Agent": this._secureAuthenticator.agents.browserUserAgent,
                "CUSTOM_USER_AGENT": this._secureAuthenticator.agents.userAgent,
                "Content-Type": "application/x-www-form-urlencoded",
                "Cookie": `JSESSIONID=${this._secureAuthenticator.cookie};`,
            }
        });

        if (smsResponse.data.headerData.status !== 200) throw new Error(`Failed to send SMS authentication: ${smsResponse.data.headerData.errorMessage} (${smsResponse.data.headerData.errorCode})`);

        this._config = config;
        this._sent = true;
    }

    async verify(code: string) {
        if (!this.sent) throw new Error("Please request SMS authentication first.");

        if (!this._config) throw new Error("SMS configuration is missing.");

        const response = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM001200503_P02.do", {
            mbphEnprDvCd: this._config.telecom,
            mbphNo: this._secureAuthenticator.details.phone,
            mbleCertNo: code,
            isMyDevice: "Y"
        }, {
            headers: {
                "User-Agent": this._secureAuthenticator.agents.browserUserAgent,
                "CUSTOM_USER_AGENT": this._secureAuthenticator.agents.userAgent,
                "Content-Type": "application/x-www-form-urlencoded",
                "Cookie": `JSESSIONID=${this._secureAuthenticator.cookie};`,
            }
        });

        if (response.data.headerData.status !== 200) throw new Error(`Failed to verify SMS authentication: ${response.data.headerData.errorMessage} (${response.data.headerData.errorCode})`);

        this._verified = true;
    }

    createPINRegistrar(pin: string) {
        if (!this._verified) throw new Error("SMS authentication must be verified first.");
        if (!this._secureAuthenticator.cookie) throw new Error("SecureSMSAuthenticator instance was not initialized");

        return new KebHanaPINRegistrar(pin, this._secureAuthenticator.cookie, this._secureAuthenticator.uuid);
    }

    get sent() {
        return this._sent;
    }

    get verified() {
        return this._verified;
    }
};