type t = {
  authorization_endpoint: string,
  token_endpoint: string,
  jwks_uri: string,
  userinfo_endpoint: string,
};

let parseDiscover = json =>
  Yojson.Basic.Util.{
    authorization_endpoint:
      json |> member("authorization_endpoint") |> to_string,
    token_endpoint: json |> member("token_endpoint") |> to_string,
    jwks_uri: json |> member("jwks_uri") |> to_string,
    userinfo_endpoint: json |> member("userinfo_endpoint") |> to_string,
  };

let fromResponse = body => Yojson.Basic.from_string(body) |> parseDiscover;

let makeRequest = () => {
  open Lwt_result.Infix;

  Logs.app(m => m("Starting discover request"));

  let https_url = "https://login.microsoftonline.com/common/v2.0/.well-known/openid-configuration";
  Logs.app(m => m("Requesting: %s", https_url));

  let req =
    Httpkit.Client.Request.create(
      ~headers=[("User-Agent", "ReVouch")],
      `GET,
      https_url |> Uri.of_string,
    );

  Httpkit_lwt.Client.(Https.send(req) >>= Response.body)
  |> Lwt.map(res =>
       switch (res) {
       | Ok(body) =>
         // Logs.app(m => m("Response: %s", body));
         fromResponse(body)
       | Error(_) =>
         // Logs.err(m => m("Something went wrong!!!"));
         {
           authorization_endpoint: "string",
           token_endpoint: "string",
           jwks_uri: "string",
           userinfo_endpoint: "string",
         }
       }
     );
};
