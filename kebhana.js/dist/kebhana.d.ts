import { CustomerInfo, HanaPrivateKeyLoginConfig, Transaction, TransactionsConfig, TransferRequest, TransferRequestConfig } from "./types.js";
export declare class KebHana {
    private _cookie;
    private _pin;
    private berry;
    private _privateKey;
    uuid: string;
    private static pinSignURL;
    private userAgent;
    private browserUserAgent;
    private secret;
    private bin;
    private cryptoKey;
    private _customerInfo;
    constructor(_cookie: string, _pin: string, berry: string, _privateKey: string, uuid?: string);
    static loginByPrivateKey(config: HanaPrivateKeyLoginConfig): Promise<KebHana>;
    private request;
    transactions(config: TransactionsConfig): Promise<Transaction[]>;
    initiateTransfer(config: TransferRequestConfig): Promise<TransferRequest>;
    fetchCustomerInfo(): Promise<CustomerInfo>;
    get customerInfo(): CustomerInfo | null;
    processTransfer(request: TransferRequest, passcode: string): Promise<string>;
}
