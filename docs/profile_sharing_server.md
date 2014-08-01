Profile Sharing Server-Side Integration
=======================================

Once a user has authorized your app to use their PayPal account, you can then use that authorization to obtain tokens that let you retrieve PayPal information for that user.

This document covers how to:

1. [Obtain OAuth2 Tokens](#obtain-oauth2-tokens)
2. [Retrieve Customer Information](#retrieve-customer-information) using those tokens

_See [Profile Sharing](profile_sharing_mobile.md) to let users authenticate and consent to profile sharing in your app._


Obtain OAuth2 Tokens
--------------------

1. [Obtain an Authorization Code](#obtain-an-authorization-code)
2. [Exchange Authorization Code for Refresh / Access Tokens](#exchange-authorization-code-for-refresh--access-tokens)

### Obtain an Authorization Code

The Mobile SDK provides [an API to obtain user consent for profile sharing](profile_sharing_mobile.md). Integrate the SDK into your app and use it to authenticate the user and obtain the user's consent. The SDK handles authentication and authorization with the PayPal authentication server, and returns an OAuth2 authorization code to your application.

This authorization response is a JSON Object. Example:

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

Your application is responsible for communicating this authorization response to your server. Your server should exchange the authorization code for refresh and access tokens, as described in the next section.


### Exchange Authorization Code for Refresh / Access Tokens

Once your server has obtained the authorization code, you can use it to get a long-lived refresh token and short-lived access token. The interaction between your server and the PayPal APIs is a standard [OAuth2 Authorization Code Grant Access Request](http://tools.ietf.org/html/rfc6749#section-4.1.3).

The authorization code is very short-lived. You should exchange it for a refresh and access token **immediately**.

The refresh token is long-lived (currently 10 years). You should store it securely.

Note that all returned code and token values should be considered variable length, and may increase in length in the future, so do not assume a fixed maximum.


#### Request

```bash
curl 'https://api.paypal.com/v1/oauth2/token' \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -H "Authorization: Basic QWZV...==" \
    -d 'grant_type=authorization_code&response_type=token&redirect_uri=urn:ietf:wg:oauth:2.0:oob&code=EBYhRW3ncivudQn8UopLp4A28...'
```

* `grant_type`: Token grant type. Value must be set to `authorization_code`.
* `redirect_uri`: Redirect uri. Value must be set to `urn:ietf:wg:oauth:2.0:oob`.
* `code`: Authorization code previously received from the authorization server.

#### Response

```JSON
{
    "access_token": "6oyryV79E.KtpAvPudpI8VIko.ntdPikU9HCDfg0tO0",
    "expires_in": 900,
    "refresh_token": "MFYQJTPW3zlCAjznPs2D0VQlQXwiEfTesR-dRiU_qhbUngzxR3NmeBxqKELcmGtSI739R-awwvOyGVO1LJbowy7n8Ul3vsf5HQDTCzUlDylqBvW0",
    "scope": "https://api.paypal.com/v1/payments/.* https://uri.paypal.com/services/payments/futurepayments",
    "token_type": "Bearer"
}
```

* `access_token`: The access token issued by the authorization server.
* `expires_in`: The lifetime of the access token in seconds. After the access token expires, use the `refresh_token` to [refresh the access token](#refresh-an-access-token).
* `refresh_token`: The refresh token, which can be used to obtain new access tokens using the same authorization grant, as described in [OAuth2.0 RFC6749 - Section 6](http://tools.ietf.org/html/rfc6749#section-6).
* `scope`: A list of space-separated scope-values associated with this refresh token.
* `token_type`: The type of the token issued as described in [OAuth2.0 RFC6749 - Section 7.1](http://tools.ietf.org/html/rfc6749#section-7.1). Value is case insensitive.


Retrieve Customer Information
-----------------------------

When your server wishes to retrieve PayPal information for a user who has granted consent, your server code will need to:

1. Look up the user's refresh token and [use it to get a new access token](#refresh-an-access-token)
2. [Retrieve customer information using that valid access token](#retrieve-customer-information-using-a-valid-access-token)

### Refresh an Access Token

As mentioned earlier, an access token is only valid for a limited time. Once it expires, you'll need to get a new access token using the refresh token.

#### Request

```bash
curl 'https://api.paypal.com/v1/oauth2/token' \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -H "Authorization: Basic QWZVa...==" \
    -d 'grant_type=refresh_token&refresh_token=MFYQ...'
```

* `grant_type`: Token grant type. Value must be set to `refresh_token`. **Required.**
* `refresh_token`: Refresh token previously received along with the access token that is to be refreshed. **Required.**

#### Response

```JSON
{
    "access_token": "WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w",
    "app_id": "APP-3TS46380HB829954H",
    "expires_in": 900,
    "scope": "https://api.paypal.com/v1/payments/.* https://uri.paypal.com/services/payments/futurepayments",
    "token_type": "Bearer"
}
```


### Retrieve Customer Information Using a Valid Access Token

Use an access token as the value of the HTTP `Authorization` header to retrieve the ifnformation, e.g.

```
Authorization: Bearer YOUR_ACCESS_TOKEN
```

* For more information, see [Get user information](https://developer.paypal.com/docs/api/#get-user-information).

For example, to obtain all of the information the user consented to share, use a request similar to this:

#### Request

```bash
curl -v https://api.paypal.com/v1/identity/openidconnect/userinfo/?schema=openid \
    -H "Content-Type:application/json" \
    -H "Authorization: Bearer WfXdnxmyJtdF4q59ofxuQuAAk6eEV-Njm6puht3Nk3w"
```

#### Response

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


Related information:

- [Get an access token](https://developer.paypal.com/webapps/developer/docs/integration/direct/make-your-first-call/#get-an-access-token).
- [Login With PayPal Integration Guide](https://developer.paypal.com/docs/integration/direct/identity/log-in-with-paypal/).
- [Get user information](https://developer.paypal.com/docs/api/#get-user-information).