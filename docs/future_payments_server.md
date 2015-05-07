Future Payments Server-Side Integration
=======================================

Once a user has authorized your app to use their PayPal account, you can then use that authorization to obtain tokens that let you create future payments from that user.

This document covers how to:

1. [Obtain OAuth2 Tokens](#obtain-oauth2-tokens)
2. [Create a Payment](#create-a-payment) using those tokens

_See [Future Payments](future_payments_mobile.md) to let users authenticate and consent to future payments in your app._


Obtain OAuth2 Tokens
--------------------

1. [Obtain an Authorization Code](#obtain-an-authorization-code)
2. [Exchange Authorization Code for Refresh / Access Tokens](#exchange-authorization-code-for-refresh--access-tokens)

### Obtain an Authorization Code

The Mobile SDK provides [an API to obtain user consent for future payments](future_payments_mobile.md). Integrate the SDK into your app and use it to authenticate the user and obtain the user's consent. The SDK handles authentication and authorization with the PayPal authentication server, and returns an OAuth2 authorization code to your application.

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
    -u "client_id:secret" \
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


Create a Payment
----------------

When a user who has granted consent later initiates a payment, your server code will need to:

1. Look up the user's refresh token and [use it to get a new access token](#refresh-an-access-token)
2. [Create a payment using that valid access token](#create-a-payment-using-a-valid-access-token)

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


### Create a Payment Using a Valid Access Token

Use an access token as the value of the HTTP `Authorization` header to complete the payment, e.g.

```
Authorization: Bearer YOUR_ACCESS_TOKEN
```

When creating a payment, you can either do a direct sale or an authorization that you later capture.

Tips:

* For more information, see [creating a payment](https://developer.paypal.com/webapps/developer/docs/api/#create-a-payment).

* When a payment is initiated from a mobile device, all payment API requests should include the `PayPal-Client-Metadata-Id` header value obtained using the [Mobile SDK](future_payments_mobile.md#obtain-a-client-metadata-id).

* Unlike the standard REST API docs that demonstrate a one time payment, a future payment doesn't require you to separately [get payment approval](https://developer.paypal.com/webapps/developer/docs/integration/web/accept-paypal-payment/#get-payment-approval) after getting initial user consent. The payment is pre-approved by the user.


### Example: Authorize then Capture

For example, to first [authorize the payment](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#authorize-the-payment), use a request similar to this:

#### Request

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

#### Response

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

Once your service is complete or goods provided, you would then [capture the payment](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment):

#### Request

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

#### Response

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

The sale is complete when the `state` is shown as `completed`.


### Refunds and Voids

For refunds and voids, simply use an access token obtained via a [client_credentials request](https://developer.paypal.com/webapps/developer/docs/api/#authentication--headers). (Do not use the user-authorized access token generated from a refresh token, as it will not have sufficient scope.)

Related information:

- [Get an access token (for refunds and voids) ](https://developer.paypal.com/webapps/developer/docs/integration/direct/make-your-first-call/#get-an-access-token).
- [Look up a completed sale](https://developer.paypal.com/docs/api/#look-up-a-sale)
- [Refund a direct sale](https://developer.paypal.com/docs/api/#refund-a-sale)
- [Look up an authorization](https://developer.paypal.com/docs/api/#look-up-an-authorization)
- [Void an authorization](https://developer.paypal.com/docs/api/#void-an-authorization)
- [Reauthorize a payment](https://developer.paypal.com/docs/api/#reauthorize-a-payment)
- [Look up a captured payment](https://developer.paypal.com/docs/api/#look-up-a-captured-payment)
- [Refund a captured payment](https://developer.paypal.com/docs/api/#refund-a-captured-payment)
