import { createHmac, generateKeyPairSync, randomBytes, randomUUID } from "crypto";
import { WizveraCrypto } from "../wizvera/wizveracrypto.js";
import axios from "axios";
import jws from "jws";
export class KebHanaPINRegistrar {
    pin;
    _cookie;
    uuid;
    pinSignURL = "https://mbp.hanabank.com/wizvera/pinsign/svc/pinsign_service.jsp";
    userAgent;
    browserUserAgent = "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148";
    bin;
    secret;
    cryptoKey;
    crypto1;
    crypto2;
    _ecPublic;
    _ecPrivate;
    _jwk;
    constructor(pin, _cookie, uuid = randomUUID()) {
        this.pin = pin;
        this._cookie = _cookie;
        this.uuid = uuid;
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
        if (!/^[0-9]{6}$/.test(pin))
            throw new Error("Invalid PIN format. Please provide a 6-digit PIN.");
        this.secret = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${this.pin}`), 512);
        this.bin = WizveraCrypto.sha256(this.secret, 512);
        this.cryptoKey = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${this.bin.toString("base64url")}`), 1024);
        this.crypto1 = new WizveraCrypto(this.cryptoKey.toString("base64url"));
        this.crypto2 = new WizveraCrypto(this.secret.toString("base64url"));
        const key = generateKeyPairSync("ec", {
            namedCurve: "P-256",
            publicKeyEncoding: {
                type: "spki",
                format: "der"
            },
            privateKeyEncoding: {
                type: "pkcs8",
                format: "pem"
            }
        });
        this._ecPublic = Buffer.from(key.publicKey);
        const x = Uint8Array.prototype.slice.call(this._ecPublic).slice(27, 59);
        const y = Uint8Array.prototype.slice.call(this._ecPublic).slice(59, 91);
        this._jwk = {
            kty: "EC",
            crv: "P-256",
            x: Buffer.from(x).toString("base64url"),
            y: Buffer.from(y).toString("base64url")
        };
        this._ecPublic = key.publicKey;
        this._ecPrivate = key.privateKey;
    }
    async register() {
        const { data: preprocessing } = await axios.post("https://mbp.hanabank.com/CREG/CREG01/CREG010100801.do", {}, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        if (preprocessing.headerData.status !== 200)
            throw new Error(`Failed to preprocess PIN registration: ${preprocessing.headerData.errorMessage} (${preprocessing.headerData.errorCode})`);
        const { data: pinConfirm } = await axios.post("https://mbp.hanabank.com/CCOM/CCOM00/CCOM000200401_P01.do", {
            simplePw: this.pin,
            simplePwConfirm: this.pin
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        if (pinConfirm.headerData.status !== 200)
            throw new Error(`Failed to confirm PIN: ${pinConfirm.headerData.errorMessage} (${pinConfirm.headerData.errorCode})`);
        const pinSecret = randomBytes(32);
        const { data: resetPin } = await axios.post("https://mbp.hanabank.com/common/pinsignRegister.do", {
            resetPinYn: "Y"
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        if (resetPin.headerData.status !== 200)
            throw new Error(`Failed to reset PIN: ${resetPin.headerData.errorMessage} (${resetPin.headerData.errorCode})`);
        const custNo = resetPin.data.custNo;
        const authCode = resetPin.data.authCode;
        const berryName = resetPin.data.berryName;
        const refValue = resetPin.data.refValue;
        const { data: { time } } = await axios.post(this.pinSignURL, {
            action: "time"
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        const { data: issueNew } = await axios.post(this.pinSignURL, {
            osInfo: "IOS",
            action: "issueNew",
            pinSecret: this.crypto1.encrypt(Buffer.from(this.crypto2.encrypt(pinSecret).toString("base64url"))).toString("base64url"),
            storage: "INAPP",
            deviceId: this.uuid,
            deviceModelName: "iPhone16,1",
            pinType: "PIN",
            berryReq: jws.sign({
                header: {
                    alg: "HS256",
                    cty: "BERRY ISSUE",
                    kid: refValue,
                    sat: time
                },
                payload: JSON.stringify({
                    req: jws.sign({
                        header: {
                            alg: "ES256",
                            cty: "BERRY REQ",
                            sat: time
                        },
                        payload: JSON.stringify({
                            jwk: JSON.stringify(this._jwk),
                            sub: berryName,
                        }),
                        secret: this._ecPrivate
                    })
                }),
                secret: Buffer.from(authCode, "base64url")
            }),
            hashPin: this.bin.toString("base64url")
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        if (issueNew.resultCode !== 0)
            throw new Error(`Failed to issue new PIN: ${issueNew.errMsg}`);
        const { data: nonceData } = await axios.post(this.pinSignURL, {
            action: "nonce"
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        await axios.post(this.pinSignURL, {
            authType: "PIN",
            action: "secret",
            nonceMac: createHmac("sha256", this.cryptoKey.toString("base64url")).update(nonceData.nonce).digest("base64url"),
            usage: "sign",
            berry: issueNew.berry,
        }, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        const { data: anonym } = await axios.post("https://mbp.hanabank.com/login/anonymSession.do", {}, {
            headers: {
                "Cookie": `JSESSIONID=${this._cookie}`,
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        });
        if (anonym.headerData.status !== 200)
            throw new Error(`Failed to retrive HWID: ${anonym.headerData.errorMessage} (${anonym.headerData.errorCode})`);
        return {
            berry: issueNew.berry,
            privateKey: this._ecPrivate,
            custNo,
            uuid: anonym?.data?.anonymSession?.tempMap?.machUnqNo || this.uuid
        };
    }
    get ecPublic() {
        return this._ecPublic;
    }
    get ecPrivate() {
        return this._ecPrivate;
    }
    get jwk() {
        return this._jwk;
    }
}
;
