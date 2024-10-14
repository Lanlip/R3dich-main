import { randomUUID } from "crypto";
import { SecureSMSAuthenticationRequest, SecureSMSAuthenticationStatus } from "../types.js";
import axios from "axios";

export class SecureSMSAuthenticator {
    private _cookie: string | null = null;
    private userAgent: string;
    private browserUserAgent: string = "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148";
    private _authenticationRequest: SecureSMSAuthenticationRequest | null = null;
    private _verified: boolean = false;
    private _details: { verified: boolean, phone: string } = { verified: false, phone: "" };

    constructor(public uuid: string = randomUUID()) {
        this.userAgent = JSON.stringify({
            model: "iPhone 12 mini",
            subscriberId: "",
            appVersion: "2.57",
            terminalInfoId: this.uuid,
            userAgent: this.browserUserAgent,
            telecom: "",
            deviceWidth: "375.0",
            deviceHeight: "812.0",
            simSerialNumber: "",
            deviceId: "",
            platform: "iPhone",
            brand: "Apple",
            phoneNumber: "",
            phoneName: "",
            countryIso: "en",
            version: "18.1",
            uid: this.uuid,
            appName: "OneQPlus",
            hUid: this.uuid,
            etcStr: ""
        });
    }

    async initialize(): Promise<SecureSMSAuthenticationRequest> {
        const response = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM001200401_P01.do", {
            uuidIos: this.uuid
        }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent
            }
        });

        if (response.data.headerData.status !== 200) {
            throw new Error(`Failed to initialize Secure SMS authentication: ${response.data.headerData.errorMessage} (${response.data.headerData.errorCode})`);
        }
        
        this._cookie = response.headers["set-cookie"]?.find((cookie: string) => cookie.startsWith("JSESSIONID"))?.split("=")[1].split(";")[0] || null;

        this._authenticationRequest = response.data.data as SecureSMSAuthenticationRequest;

        return this._authenticationRequest;
    }

    async check(): Promise<SecureSMSAuthenticationStatus> {
        if (!this._authenticationRequest) {
            throw new Error("Secure SMS authentication has not been initialized yet.");
        }

        const response = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM001200402_P01.do", {
            rqstNo: this._authenticationRequest.rqstNo
        }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
                "Cookie": `JSESSIONID=${this._cookie}`
            }
        });

        if (response.data.headerData.status !== 200) throw new Error(`Failed to check authentication status: ${response.data.headerData.errorMessage} (${response.data.headerData.errorCode})`);

        const verified = response.data?.data?.scm0001?.rspsRsltCd === "M000";
        const phone = response.data?.data?.scm0001?.mbphNo;

        this._verified = verified;
        this._details = { verified, phone };

        return this._details;
    }

    get agents() {
        return {
            userAgent: this.userAgent,
            browserUserAgent: this.browserUserAgent
        };
    }

    get details() {
        return this._details;
    }

    get verified() {
        return this._verified;
    }

    get authenticationRequest() {
        return this._authenticationRequest;
    }

    get cookie() {
        return this._cookie;
    }
};