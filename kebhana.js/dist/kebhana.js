import axios from "axios";
import { createHmac, randomUUID } from "crypto";
import jws from "jws";
import { WizveraCrypto } from "./wizvera/wizveracrypto.js";
import { formatEpochToKST } from "./utils.js";
var UserAgentHeaderType;
(function (UserAgentHeaderType) {
    UserAgentHeaderType[UserAgentHeaderType["App"] = 0] = "App";
    UserAgentHeaderType[UserAgentHeaderType["Browser"] = 1] = "Browser";
})(UserAgentHeaderType || (UserAgentHeaderType = {}));
;
export class KebHana {
    _cookie;
    _pin;
    berry;
    _privateKey;
    uuid;
    static pinSignURL = "https://mbp.hanabank.com/wizvera/pinsign/svc/pinsign_service.jsp";
    userAgent;
    browserUserAgent = "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148";
    secret;
    bin;
    cryptoKey;
    _customerInfo = null;
    constructor(_cookie, _pin, berry, _privateKey, uuid = randomUUID()) {
        this._cookie = _cookie;
        this._pin = _pin;
        this.berry = berry;
        this._privateKey = _privateKey;
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
        this.secret = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${_pin}`), 512);
        this.bin = WizveraCrypto.sha256(this.secret, 512);
        this.cryptoKey = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${this.bin.toString("base64url")}`), 1024);
    }
    static async loginByPrivateKey(config) {
        if (!config)
            throw new Error("HanaPrivateKeyLoginConfig is required.");
        const secret = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${config.pin}`), 512);
        const bin = WizveraCrypto.sha256(secret, 512);
        const cryptoKey = WizveraCrypto.sha256(Buffer.from(`WIZVERA ID2${bin.toString("base64url")}`), 1024);
        const { data: pinsignNonce, headers } = await axios.post("https://mbp.hanabank.com/common/pinsignNonce.do", { custNo: config.custNo }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
                "CUSTOM_USER_AGENT": JSON.stringify({
                    model: "iPhone 12 mini",
                    subscriberId: "",
                    appVersion: "2.57",
                    terminalInfoId: config.uuid,
                    userAgent: "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
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
                    uid: config.uuid,
                    appName: "OneQPlus",
                    hUid: config.uuid,
                    etcStr: ""
                })
            }
        });
        const cookie = headers["set-cookie"]?.find((cookie) => cookie.includes("JSESSIONID"))?.split(";")[0].split("=")[1];
        if (!cookie)
            throw new Error("Failed to retrieve JSESSIONID.");
        const { data: nonceData } = await axios.post(this.pinSignURL, {
            action: "nonce"
        }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
                "Cookie": `JSESSIONID=${cookie}`,
                "CUSTOM_USER_AGENT": JSON.stringify({
                    model: "iPhone 12 mini",
                    subscriberId: "",
                    appVersion: "2.57",
                    terminalInfoId: config.uuid,
                    userAgent: "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
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
                    uid: config.uuid,
                    appName: "OneQPlus",
                    hUid: config.uuid,
                    etcStr: ""
                })
            }
        });
        await axios.post(this.pinSignURL, {
            authType: "PIN",
            action: "secret",
            nonceMac: createHmac("sha256", cryptoKey.toString("base64url")).update(nonceData.nonce).digest("base64url"),
            usage: "sign",
            berry: config.berry,
        }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
                "Cookie": `JSESSIONID=${cookie}`,
                "CUSTOM_USER_AGENT": JSON.stringify({
                    model: "iPhone 12 mini",
                    subscriberId: "",
                    appVersion: "2.57",
                    terminalInfoId: config.uuid,
                    userAgent: "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
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
                    uid: config.uuid,
                    appName: "OneQPlus",
                    hUid: config.uuid,
                    etcStr: ""
                })
            }
        });
        const { data: response } = await axios.post("https://mbp.hanabank.com/login/mbpLogin00J.do", {
            jws: jws.sign({
                header: {
                    alg: "ES256",
                    BERRY: config.berry,
                    authtp: "K1",
                    nonce: pinsignNonce.data.pinsignNonce,
                    psign: "3.1.0",
                    sat: nonceData.time,
                    wcrypto: "1.3.2"
                },
                payload: "login=true",
                secret: config.privateKey
            })
        }, {
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
                "Cookie": `JSESSIONID=${cookie}`,
                "CUSTOM_USER_AGENT": JSON.stringify({
                    model: "iPhone 12 mini",
                    subscriberId: "",
                    appVersion: "2.57",
                    terminalInfoId: config.uuid,
                    userAgent: "Mozilla/5.0 (iPhone; CPU iPhone OS 18_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
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
                    uid: config.uuid,
                    appName: "OneQPlus",
                    hUid: config.uuid,
                    etcStr: ""
                })
            }
        });
        if (response.headerData.status !== 200)
            throw new Error(`Failed to login: ${response.headerData.errorMessage} (${response.headerData.errorCode})`);
        return new KebHana(cookie, config.pin, config.berry, config.privateKey, config.uuid);
    }
    request({ method, url, data, headers }, headerType = UserAgentHeaderType.Browser) {
        return axios({
            method,
            url,
            data,
            headers: {
                Cookie: `JSESSIONID=${this._cookie};`,
                "Content-Type": "application/x-www-form-urlencoded",
                ...(headerType === UserAgentHeaderType.Browser ? {
                    "User-Agent": this.browserUserAgent,
                    "CUSTOM_USER_AGENT": this.userAgent,
                } : {
                    "User-Agent": this.userAgent,
                    "CUSTOM_USER_AGENT": this.browserUserAgent
                }),
                ...headers,
            }
        });
    }
    async transactions(config) {
        const now = new Date();
        const bef = new Date(now.getFullYear(), now.getMonth(), now.getDate() - 30);
        const { data } = await this.request({
            method: "POST",
            url: "https://mbp.hanabank.com/BINQ/BINQ01/BINQ010000001.do",
            data: {
                acctNo: config.accountNumber,
                acctSeq: "",
                curCd: config.accountCurrency || "KRW",
                wdrwDstnDt: "",
                trscSeqNo: "",
                dtlsSeqNo: "",
                inqStrDt: config.startDate || `${bef.getFullYear().toString().padStart(2, "0")}${(bef.getMonth() + 1).toString().padStart(2, "0")}${bef.getDate().toString().padStart(2, "0")}`,
                inqEndDt: config.endDate || `${now.getFullYear().toString().padStart(2, "0")}${(now.getMonth() + 1).toString().padStart(2, "0")}${now.getDate().toString().padStart(2, "0")}`,
                inqRqreNcnt: config.limit || "100",
                rvSeqInqYn: "Y",
                rcvWdrwDvCd: "0",
                fastInqYn: "N",
                srchStrCtt: "",
                trscPtclSrchDvCd: "",
                pageNo: config.page || "0"
            },
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent
            }
        });
        if (data.headerData.status !== 200)
            throw new Error(`Failed to fetch transactions: ${data.headerData.errorMessage} (${data.headerData.errorCode})`);
        const _txs = data.data.SNBSIQ000101.contMap["BIZ.SIQ0001.OUT.REC1"];
        return _txs.map((tx) => {
            return {
                memo: tx.rmrk,
                amount: tx.trscAmt,
                description: tx.apltNm,
                channel: {
                    name: tx.chnlTypNm,
                    code: tx.chnlTypCd,
                },
                finalBalance: tx.trscAfBal,
                isDeposit: tx.balFlctDvCd === "1",
                isWithdraw: tx.balFlctDvCd === "2",
                transactionId: tx.globId,
                currency: {
                    code: tx.curCd,
                    name: tx.curCd_Name,
                },
                thirdPartyBank: {
                    name: tx.thrBnkCd_Name,
                    code: tx.thrBnkCd,
                    accountNumber: tx.thrAcctNo,
                },
                treatment: {
                    branchName: tx.trtBrNm,
                    branchCode: tx.trtBrNo,
                },
            };
        });
    }
    async initiateTransfer(config) {
        const { data } = await this.request({
            method: "POST",
            url: "https://mbp.hanabank.com/BTRN/BTRN01/BTRN010100111.do",
            data: {
                chnlSvcCd: "A16",
                paymAcctNo: config.from,
                rcvBnkCd: config.bankCode,
                rcvAcctNo: config.to,
                trnsAmt: config.amount,
                wdrwAcctSeqNo: "1",
                wdrwBnkCd: "081",
                trnsSeqNo: "1",
                addTransferYn: "N",
                svcDvCd: "ACCT_TRNS",
                rmtYN: "N"
            },
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                "CUSTOM_USER_AGENT": this.userAgent,
                "User-Agent": this.browserUserAgent,
            }
        }, UserAgentHeaderType.Browser);
        if (data.headerData.status !== 200)
            throw new Error(`Failed to initiate transfer: ${data.headerData.errorMessage} (${data.headerData.errorCode})`);
        const tx = data?.data?.imdtTrns;
        if (!tx)
            throw new Error("Failed to retrieve immediate transfer data.");
        const { data: update } = await this.request({
            method: "POST",
            url: "https://mbp.hanabank.com/BTRN/BTRN01/BTRN010100112.do",
            data: {
                trnsSeqNo: tx.wdrwAcctSeqNo,
                pintPrdLclasCd: "",
                cshbUseYn: "",
                cshbUseAmt: "0",
                rcvPsbkMarkCtt: config.depositorName || tx.rcvPsbkMarkCtt,
                wdrwPsbkMarkCtt: config.memo || tx.wdrwPsbkMarkCtt,
                spclMttr: "",
                slctDvCd: "",
                trnsScheDt: "",
                trnsScheTm: "",
                custNtfyMdclCd: "",
                mmdaHoldYn: "N"
            }
        });
        if (update.headerData.status !== 200)
            throw new Error(`Failed to update transfer: ${update.headerData.errorMessage} (${update.headerData.errorCode})`);
        return {
            from: `${tx.wdrwAcctNo.slice(0, 3)}-${tx.wdrwAcctNo.slice(3, 10)}-${tx.wdrwAcctNo.slice(10)}`,
            to: tx.trnsList[0].rcvAcctNo,
            amount: tx.totlTrnsAmt,
            fee: tx.totlRduAfComm,
            bankCode: config.bankCode,
            depositorName: config.depositorName || tx.trnsList[0].rcvPsbkMarkCtt,
            memo: config.memo || tx.trnsList[0].wdrwPsbkMarkCtt,
            receiver: tx.trnsList[0].rmteNm,
            sender: tx.trnsList[0].rmtrNm,
            transactionId: tx.tmsgUnqNo13
        };
    }
    async fetchCustomerInfo() {
        const { data } = await this.request({
            url: "https://mbp.hanabank.com/login/loginSession.do",
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            data: {
                key: "custInfo"
            }
        });
        if (data.headerData.status !== 200)
            throw new Error(`Failed to fetch customer info: ${data.headerData.errorMessage} (${data.headerData.errorCode})`);
        this._customerInfo = {
            custNo: data.data.custInfo.custNo,
            ssn: data.data.custInfo.rsbzRegNo,
            name: data.data.custInfo.custNm,
            phone: data.data.custInfo.mbphNo,
            address: data.data.custInfo.adr,
            englishName: data.data.custInfo.custEngNm,
            userId: data.data.custInfo.userId,
            email: data.data.custInfo.emalAdr
        };
        return this._customerInfo;
    }
    get customerInfo() {
        return this._customerInfo;
    }
    async processTransfer(request, passcode) {
        const custData = this._customerInfo || await this.fetchCustomerInfo();
        const { data: verify } = await this.request({
            method: "POST",
            url: "https://mbp.hanabank.com/BTRN/BTRN00/BTRN000000002.do",
            data: {
                type: "1",
                acctNo: request.from.replace(/-/g, ""),
                trnsAmt: request.amount.toString(),
                svcDvCd: "ACCT_TRNS",
                pwVrfcYn: "Y",
                acctPw: passcode
            }
        });
        if (verify.headerData.status !== 200)
            throw new Error(`Failed to verify passcode: ${verify.headerData.errorMessage} (${verify.headerData.errorCode})`);
        const { data: pinsignNonce } = await this.request({
            method: "POST",
            url: "https://mbp.hanabank.com/common/pinsignNonce.do",
            data: {
                custNo: custData.custNo,
                mbleSvcDvCd: "5"
            }
        });
        const { data: { time } } = await this.request({
            url: KebHana.pinSignURL,
            method: "POST",
            data: {
                action: "time"
            }
        });
        await this.request({
            url: "https://mbp.hanabank.com/common/addAuthMethodInquiry.do",
            method: "POST",
            data: {
                highSecurityGb: "transfer"
            }
        });
        const { data: transfer } = await this.request({
            url: "https://mbp.hanabank.com/BTRN/BTRN01/BTRN010100114.do",
            method: "POST",
            data: {
                svcDvCd: "ACCT_TRNS",
                bizDvNm: "transfer",
                tmsgUnqNo13: request.transactionId,
                signed_msg: jws.sign({
                    header: {
                        BERRY: this.berry,
                        alg: "ES256",
                        authtp: "K4",
                        nonce: pinsignNonce.data.pinsignNonce,
                        psign: "3.1.0",
                        sat: time,
                        wcrypto: "1.3.2"
                    },
                    payload: {
                        "출금계좌번호[wdrwAcctNo_1]": request.from,
                        "입금은행코드[rcvBnkCd_1]": request.bankCode,
                        "입금계좌번호[rcvAcctNo_1]": request.to,
                        "이체금액[trnsAmt_1]": request.amount,
                        "수수료[rduAfComm_1]": request.fee,
                        "받는분에게표기[rcvPsbkMarkCtt_1]": request.depositorName,
                        "나에게표기[wdrwPsbkMarkCtt_1]": request.memo,
                        "수취인명[rmteNm_1]": request.receiver,
                        "송금인명[rmtrNm_1]": request.sender,
                        "하나머니사용여부[cshbUseYn_1]": "",
                        "하나머니사용금액[cshbUseAmt_1]": "0",
                        "메모[spclMttr_1]": "",
                        "CMS코드/모집인코드[slctDvCd_1]": "",
                        "사설인증종류코드[sign_privateCertType]": "",
                        "사설인증SKIP여부[sign_prvtCertYn]": "",
                        "전자서명데이터생성시간[sign_sslsignctime]": formatEpochToKST(time),
                    },
                    secret: this._privateKey
                }),
                vid_msg: "undefined"
            }
        });
        if (transfer.headerData.status !== 200)
            throw new Error(`Failed to process transfer: ${transfer.headerData.errorMessage} (${transfer.headerData.errorCode})`);
        return transfer.data.imdtTrns.trnsList[0].EChnlTrscAcpnNo;
    }
}
;
