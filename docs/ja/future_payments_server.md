今後の支払い(Future Payments)のためのサーバー側の統合
=======================================

ユーザーが、自分のPayPalアカウントの使用をアプリに承認すると、この承認を使用して、そのユーザーからの今後の支払いを作成するトークンを取得できます。

このドキュメントには以下の方法が記載されています。

1. [OAuth2のトークンを取得する](#obtain-oauth2-tokens)
2. 取得したトークンを使用して[支払いを作成する](#create-a-payment)

_ユーザーに、アプリで今後の支払いを承認し、同意してもらうには、[今後の支払い](future_payments_mobile.md)を参照してください。_


OAuth2のトークンを取得する
--------------------

1. [認可コードを取得する](#obtain-an-authorization-code)
2. [認可コードとリフレッシュ/アクセストークンを交換する](#exchange-authorization-code-for-refresh--access-tokens)

### 認可コードを取得する

モバイルSDKは、[今後の支払いについてユーザーの承認を得るためのAPI](future_payments_mobile.md)を提供しています。アプリにSDKを統合し、それを使ってユーザーを認証し、ユーザーの同意を得ます。SDKは、PayPal認証サーバーで認証および承認を処理し、OAuth2の認可コードをアプリに返します。

この認可応答はJSONオブジェクトです。例:

```json
{
    "response_type": "authorization_code",
    "response": {
        "code": "EBYhRW3ncivudQn8UopLp4A28xIlqPDpAoqd7biDLpeGCPvORHjP1Fh4CbFPgKMGCHejdDwe9w1uDWnjPCp1lkaFBjVmjvjpFtnr6z1YeBbmfZYqa9faQT_71dmgZhMIFVkbi4yO7hk0LBHXt_wtdsw",
    },
    "client": {
        "environment": "live",
        "paypal_sdk_version": "2.0.0",
        "platform": "iOS",
        "product_name": "PayPal iOS SDK"
    }
}
```
アプリは、この認可応答をサーバーに送信する必要があります。サーバーは、次のセクションで説明するように、認可コードと、リフレッシュトークンおよびアクセストークンを交換する必要があります。


### 認可コードとリフレッシュ/アクセストークンを交換する

サーバーが認可コードを取得すると、これを使用して、有効期限の長いリフレッシュトークンと有効期限の短いアクセストークンを取得できるようになります。サーバーとPayPal APIの通信は、標準的な[OAuth2の認可コードグラントアクセスリクエスト](http://tools.ietf.org/html/rfc6749#section-4.1.3)です。

認可コードには、非常に短い有効期限が設定されています。**ただちに**認可コードをリフレッシュトークンとアクセストークンに交換する必要があります。

リフレッシュトークンの有効期限は長く、現在のところ10年です。安全に保管する必要があります。

返されるコードやトークンの値はすべて可変長として扱ってください。今後さらに長くなる可能性がありますので、最大長が固定されているものとして扱わないでください。


#### リクエスト

```bash
curl 'https://api.paypal.com/v1/oauth2/token' \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -H "Authorization: Basic QWZV...==" \
    -d 'grant_type=authorization_code&response_type=token&redirect_uri=urn:ietf:wg:oauth:2.0:oob&code=EBYhRW3ncivudQn8UopLp4A28...'
```

* `grant_type`: トークンのグラントタイプです。値は`authorization_code`に設定する必要があります。
* `redirect_uri`: リダイレクトURLです。値は`urn:ietf:wg:oauth:2.0:oob`に設定する必要があります。
* `code`: 認可サーバーから以前に受信した認可コードです。

#### 応答

```JSON
{
    "access_token": "6oyryV79E.KtpAvPudpI8VIko.ntdPikU9HCDfg0tO0",
    "expires_in": 900,
    "refresh_token": "MFYQJTPW3zlCAjznPs2D0VQlQXwiEfTesR-dRiU_qhbUngzxR3NmeBxqKELcmGtSI739R-awwvOyGVO1LJbowy7n8Ul3vsf5HQDTCzUlDylqBvW0",
    "scope": "https://api.paypal.com/v1/payments/.* https://uri.paypal.com/services/payments/futurepayments",
    "token_type": "Bearer"
}
```

* `access_token`: 認可サーバーによって発行されたアクセストークンです。
* `expires_in`: アクセストークンの秒単位での有効期限です。アクセストークンの期限切れ後、`refresh_token`を使用して[アクセストークンを更新](#refresh-an-access-token)します。
* `refresh_token`: リフレッシュトークン。[OAuth2.0 RFC6749 - セクション6](http://tools.ietf.org/html/rfc6749#section-6)で説明されているように、これにより、同じ認可グラントを使用する新しいアクセストークンを取得することができます。
* `scope`: このリフレッシュトークンに関連するスコープ値をスペースで区切ったリストです。
* `token_type`: [OAuth2.0 RFC6749 - セクション7.1](http://tools.ietf.org/html/rfc6749#section-7.1)で説明されているタイプの発行済みトークンです。値は、大文字と小文字が区別されます。


支払いの作成
----------------

同意したユーザーが後で支払いを開始した場合、サーバーコードで以下を実行する必要があります。

1. ユーザーのリフレッシュトークンを検索し、[そのトークンを使用して新しいアクセストークンを取得](#refresh-an-access-token)する
2. [有効なアクセストークンを使用して支払いを作成する](#create-a-payment-using-a-valid-access-token)

### アクセストークンの更新

前述したように、アクセストークンには有効期限があります。アクセストークンの期限が切れた場合は、リフレッシュトークンを使用して新しいアクセストークンを取得する必要があります。

#### リクエスト

```bash
curl 'https://api.paypal.com/v1/oauth2/token' \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -H "Authorization: Basic QWZVa...==" \
    -d 'grant_type=refresh_token&refresh_token=MFYQ...'
```

* `grant_type`:  トークンのグラントタイプです。値は`refresh_token`に設定する必要があります。**必須です。**
* `refresh_token`: 更新されるアクセストークンとともに以前に受信したリフレッシュトークンです。**必須です。**

#### 応答

```JSON
{
    "access_token": "WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w",
    "app_id": "APP-3TS46380HB829954H",
    "expires_in": 900,
    "scope": "https://api.paypal.com/v1/payments/.* https://uri.paypal.com/services/payments/futurepayments",
    "token_type": "Bearer"
}
```


### 有効なアクセストークンを使用した支払いの作成

次のように、アクセストークンをHTTPの`Authorization`ヘッダーの値として使用して支払いを完了します。

```
Authorization: Bearer YOUR_ACCESS_TOKEN
```

支払いの作成時に、直接の販売や後日支払いを回収する承認を行うことができます。

ヒント:

* 詳細は[支払いの作成](https://developer.paypal.com/webapps/developer/docs/api/#create-a-payment)を参照してください。

* モバイル端末から支払いが開始された場合、すべての決済APIリクエストに、[モバイルSDK]を使用して取得された`PayPal-Client-Metadata-Id`ヘッダー値(future_payments_mobile.md#obtain-an-application-correlation-id)が含まれる必要があります。

* 1回払いの実例を示す標準的なREST APIのドキュメントとは異なり、今後の支払いでは、最初にユーザーの同意を得た後は、そのつど[支払いの承認を得る](https://developer.paypal.com/webapps/developer/docs/integration/web/accept-paypal-payment/#get-payment-approval)必要はありません。支払いは、ユーザーによって事前承認されています。


### 例: 承認と回収

たとえば、最初に[支払いを承認する](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#authorize-the-payment)には、次のようなリクエストを使用します。

#### リクエスト

```bash
curl 'https://api.paypal.com/v1/payments/payment' \
    -H "Content-Type: application/json" \
    -H "PayPal-Client-Metadata-Id: c2edbd6e97b14ff2b19ddb8eec9d264c" \
    -H "Authorization: Bearer WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w" \
    -d '{
           "intent":"authorize",
           "payer":{
              "payment_method":"paypal"
           },
           "transactions":[
              {
                 "amount":{
                    "currency":"USD",
                    "total":"1.88"
                 },
                 "description":"future of sauces"
              }
           ]
        }'
```

#### 応答

```JSON
{
    "create_time": "2013-10-01T00:43:25Z",
    "id": "PAY-2C433581AX997613HKJFBVLI",
    "intent": "authorize",
    "links": [
        {
            "href": "https://api.paypal.com/v1/payments/payment/PAY-2C433581AX997613HKJFBVLI",
            "method": "GET",
            "rel": "self"
        }
    ],
    "payer": {
        "payer_info": {
            "email": "tnm10@paypal.com",
            "first_name": "George",
            "last_name": "Martin",
            "payer_id": "QLR7PGAJCMCA8"
        },
        "payment_method": "paypal"
    },
    "state": "approved",
    "transactions": [
        {
            "amount": {
                "currency": "USD",
                "details": {
                    "subtotal": "1.88"
                },
                "total": "1.88"
            },
            "description": "future of sauces",
            "related_resources": [
                {
                    "authorization": {
                        "amount": {
                            "currency": "USD",
                            "details": {
                                "subtotal": "1.88"
                            },
                            "total": "1.88"
                        },
                        "create_time": "2013-10-01T00:43:25Z",
                        "id": "4TD55050SV609544L",
                        "links": [
                            {
                                "href": "https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L",
                                "method": "GET",
                                "rel": "self"
                            },
                            {
                                "href": "https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L/capture",
                                "method": "POST",
                                "rel": "capture"
                            },
                            {
                                "href": "https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L/void",
                                "method": "POST",
                                "rel": "void"
                            },
                            {
                                "href": "https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L/reauthorize",
                                "method": "POST",
                                "rel": "reauthorize"
                            },
                            {
                                "href": "https://api.paypal.com/v1/payments/payment/PAY-2C433581AX997613HKJFBVLI",
                                "method": "GET",
                                "rel": "parent_payment"
                            }
                        ],
                        "parent_payment": "PAY-2C433581AX997613HKJFBVLI",
                        "state": "authorized",
                        "update_time": "2013-10-01T00:43:28Z",
                        "valid_until": "2013-10-30T00:43:25Z"
                    }
                }
            ]
        }
    ],
    "update_time": "2013-10-01T00:43:28Z"
}
```

サービスの完了または商品の提供後、[支払いを回収](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)します。

#### リクエスト

```bash
curl 'https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L/capture' \
    -H "Content-Type: application/json" \
    -H "Authorization: Bearer WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w" \
    -d '{
           "amount":{
              "currency":"USD",
              "total":"1.50"
           },
           "is_final_capture":true
        }'
```

#### 応答

```JSON
{
    "amount": {
        "currency": "USD",
        "total": "1.50"
    },
    "create_time": "2013-10-01T00:43:28Z",
    "id": "9BS66645698646420",
    "is_final_capture": true,
    "links": [
        {
            "href": "https://api.paypal.com/v1/payments/capture/9BS66645698646420",
            "method": "GET",
            "rel": "self"
        },
        {
            "href": "https://api.paypal.com/v1/payments/capture/9BS66645698646420/refund",
            "method": "POST",
            "rel": "refund"
        },
        {
            "href": "https://api.paypal.com/v1/payments/authorization/4TD55050SV609544L",
            "method": "GET",
            "rel": "authorization"
        },
        {
            "href": "https://api.paypal.com/v1/payments/payment/PAY-2C433581AX997613HKJFBVLI",
            "method": "GET",
            "rel": "parent_payment"
        }
    ],
    "parent_payment": "PAY-2C433581AX997613HKJFBVLI",
    "state": "completed",
    "update_time": "2013-10-01T00:43:30Z"
}
```

`state`が`completed`と表示されると、販売は完了です。


### 返金と取り消し

返金および取り消しの場合は、[client_credentialsリクエスト](https://developer.paypal.com/webapps/developer/docs/api/#authentication--headers)で取得したアクセストークンを使用します。(リフレッシュトークンから生成され、ユーザーが認証したアクセストークンは、スコープが十分でないため、使用しないでください。)

関連情報:

- [アクセストークンの取得(返金と取り消しの場合) ](https://developer.paypal.com/webapps/developer/docs/integration/direct/make-your-first-call/#get-an-access-token)
- [完了した販売の検索](https://developer.paypal.com/docs/api/#look-up-a-sale)
- [直接販売の返金](https://developer.paypal.com/docs/api/#refund-a-sale)
- [承認の検索](https://developer.paypal.com/docs/api/#look-up-an-authorization)
- [承認の取り消し](https://developer.paypal.com/docs/api/#void-an-authorization)
- [支払いの再承認](https://developer.paypal.com/docs/api/#reauthorize-a-payment)
- [回収した支払いの検索](https://developer.paypal.com/docs/api/#look-up-a-captured-payment)
- [回収した支払いの返金](https://developer.paypal.com/docs/api/#refund-a-captured-payment)
