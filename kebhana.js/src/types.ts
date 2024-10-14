export interface HeaderData {
    status: number;
    errorCode: string | null;
    errorMessage: string | null;
    errorMessageId: string | null;
    guid: string;
    url: string;
    timestamp: string;
    actionErrorList: string[];
    other?: object;
    returnUrl: string | null;
};

export interface HanaAPIResponse<T> {
    headerData: HeaderData;
    data: T;
};

export interface ARSAuthenticatorConfig {
    
};

export interface SMSAuthenticatorConfig {
    telecom: Telecom;
    name: string;
    ssnFront: string;
    ssnBack: string;
};

export enum Telecom {
    SKT = "4",
    KT = "6",
    LGU = "5",
    SKT_MVNO = "7",
    KT_MVNO = "8",
    LGU_MVNO = "A"
};

export interface HanaPrivateKeyLoginConfig {
    berry: string;
    privateKey: string;
    pin: string;
    custNo: string;
    uuid: string;
};

export interface TransactionsConfig {
    accountNumber: string;
    /**
     * 조회 계좌의 통화
     * @default "KRW"
     */
    accountCurrency?: string;
    /**
     * 조회 시작일
     * Format: YYYYMMDD
     * @example "20210101"
     * @default 30일 전
     */
    startDate?: string;
    /**
     * 조회 종료일
     * Format: YYYYMMDD
     * @example "20241231"
     * @default 현재 날짜
     */
    endDate?: string;
    /**
     * 조회 건수
     * @default 100
     */
    limit?: number;
    /**
     * 조회 페이지
     * 0 페이지 부터 시작
     * @default 0
     */
    page?: number;
};

export interface Transaction {
    memo: string;
    amount: number;
    description?: string;
    channel: {
        name: string;
        code: string;
    };
    finalBalance: number;
    isDeposit: boolean;
    isWithdraw: boolean;
    transactionId: string;
    currency: {
        code: string;
        name: string;
    };
    thirdPartyBank: {
        name: string;
        code: string;
        accountNumber: string;
    };
    treatment: {
        branchName: string;
        branchCode: string;
    };
};

export interface HanaRegisterResponse {
    privateKey: string;
    berry: string;
    pinSecret: string;
};

export interface TransferRequestConfig {
    from: string;
    to: string;
    amount: number;
    bankCode: BankCode;
    depositorName?: string;
    memo?: string;
};

export interface TransferRequest {
    transactionId: string;
    from: string;
    bankCode: BankCode;
    to: string;
    amount: number;
    fee: number;
    depositorName: string;
    memo: string;
    receiver: string;
    sender: string;
};

export interface CustomerInfo {
    custNo: string;
    ssn: string;
    name: string;
    englishName: string;
    userId: string;
    address: string;
    phone: string;
    email: string;
};

export interface SecureSMSAuthenticationRequest {
    niceNumber: string;
    msgStr: string;
    rqstNo: string;
    moEncoding: string;
};

export interface SecureSMSAuthenticationStatus {
    verified: boolean;
    phone: string;
};

export enum BankCode {
    HANA_BANK = "081",
    KOOKMIN_BANK = "004",
    WOORI_BANK = "020",
    SHINHAN_BANK = "088",
    IBK_BANK = "003",
    IM_BANK_DAEGU = "031",
    BUSAN_BANK = "032",
    KYONGNAM_BANK = "039",
    GWANGJU_BANK = "034",
    JEONBUK_BANK = "037",
    JEJU_BANK = "035",
    NONGHYUP_BANK = "011",
    KDB_BANK = "002",
    SUHYUP_BANK = "007",
    CITI_BANK_KOREA = "027",
    SC_FIRST_BANK = "023",
    HSBC_BANK = "054",
    DEUTSCHE_BANK = "055",
    BOA_BANK = "060",
    JP_MORGAN_CHASE_BANK = "057",
    ICBC_BANK = "062",
    BNP_PARIBAS_BANK = "061",
    KOREA_POST = "071",
    K_BANK = "089",
    KAKAO_BANK = "090",
    NACF = "064",
    SAVINGS_BANK = "050",
    NATIONAL_FEDERATION_OF_SMG = "045",
    CREDIT_UNION = "048",
    BANK_OF_CHINA = "063",
    CHINA_CONSTRUCTION_BANK = "067",
    TOSS_BANK = "092"
};