個人設定共有のためのサーバー側の統合
=======================================

ユーザーが、自分のPayPalアカウントの使用をアプリに承認すると、この承認を使用して、そのユーザーのPayPal情報を取り出すトークンを取得できます。

このドキュメントには以下の方法が記載されています。

1. [OAuth2のトークンを取得する](#obtain-oauth2-tokens)
2. 取得したトークンを使用して[顧客情報を取得する](#retrieve-customer-information)

_ユーザーに、アプリで個人設定の共有を承認し、同意してもらうには、[個人設定の共有](profile_sharing_mobile.md)を参照してください。_


OAuth2のトークンを取得する
--------------------

1. [認可コードを取得する](#obtain-an-authorization-code)
2. [認可コードとリフレッシュ/アクセストークンを交換する](#exchange-authorization-code-for-refresh--access-tokens)
### 認可コードを取得する

モバイルSDKは、[個人設定の共有についてユーザーの同意を得るためのAPI](profile_sharing_mobile.md)を提供しています。アプリにSDKを統合し、それを使ってユーザーを認証し、ユーザーの同意を得ます。SDKは、PayPal認証サーバーで認証および承認を処理し、OAuth2の認可コードをアプリに返します。

この認可応答はJSONオブジェクトです。例

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


顧客情報の取得
-----------------------------

サーバーが、同意を得ているユーザーのPayPal情報を取り出す必要がある場合、サーバーコードで以下を実行する必要があります。

1. ユーザーのリフレッシュトークンを検索し、[それを使用して新しいアクセストークンを取得](#refresh-an-access-token)する
2. [有効なアクセストークンを使用して顧客情報を取り出す](#retrieve-customer-information-using-a-valid-access-token)

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


### 有効なアクセストークンを使用した顧客情報の取得

HTTPの`Authorization`ヘッダーの値としてアクセストークンを使用して、次のように情報を取り出します。

```
Authorization: Bearer YOUR_ACCESS_TOKEN
```

* 詳細は、[ユーザー情報の取得](https://developer.paypal.com/docs/api/#get-user-information)を参照してください。

たとえば、ユーザーが共有に同意した全情報を取得する場合は、リクエストを次のように使用します。

#### リクエスト

```bash
curl -v https://api.paypal.com/v1/identity/openidconnect/userinfo/?schema=openid \
    -H "Content-Type:application/json" \
    -H "Authorization: Bearer WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w"
```

#### 応答

```JSON
{  
   "address":{  
      "postal_code":"95131",
      "locality":"San Jose",
      "region":"CA",
      "country":"US",
      "street_address":"3 Main St"
   },
   "family_name":"Smith",
   "language":"en_US",
   "phone_number":"4082560980",
   "locale":"en_US",
   "name":"Roger Smith",
   "email":"rsmith@somewhere.com",
   "account_type":"PERSONAL",
   "birthday":"1982-08-02",
   "given_name":"Roger",
   "user_id":"https://www.paypal.com/webapps/auth/identity/user/jG8zVpn2toXCPmzNffW1WTRLA2KOhPXYybeTM9p3ct0"
}
```


関連情報:

- [アクセストークンの取得](https://developer.paypal.com/webapps/developer/docs/integration/direct/make-your-first-call/#get-an-access-token)
- [PayPalインテグレーションガイドによるログイン](https://developer.paypal.com/docs/integration/direct/identity/log-in-with-paypal/)
- [ユーザー情報の入手](https://developer.paypal.com/docs/api/#get-user-information)