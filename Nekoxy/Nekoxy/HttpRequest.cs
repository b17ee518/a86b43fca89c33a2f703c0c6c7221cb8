using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TrotiNet;

namespace Nekoxy
{
    /// <summary>
    /// HTTPリクエストデータ。
    /// </summary>
    public class HttpRequest
    {
        /// <summary>
        /// リクエストライン、ヘッダ、ボディを元に初期化。
        /// </summary>
        /// <param name="requestLine">HTTPリクエストライン</param>
        /// <param name="headers">HTTPリクエストヘッダ</param>
        /// <param name="body">HTTPリクエストボディ</param>
        public HttpRequest(HttpRequestLine requestLine, HttpHeaders headers, byte[] body)
        {
            this.RequestLine = requestLine;
            this.Headers = headers;
            this.Body = body;
        }

        /// <summary>
        /// HTTPリクエストライン。
        /// </summary>
        public HttpRequestLine RequestLine { get; set; }

        /// <summary>
        /// HTTPヘッダ。
        /// </summary>
        public HttpHeaders Headers { get; set; }

        /// <summary>
        /// HTTPリクエストボディ。
        /// Transfer-Encoding: chunked なHTTPリクエストの RequestBody の読み取りは未対応。
        /// </summary>
        public byte[] Body { get; set; }

        /// <summary>
        /// パスとクエリ。
        /// </summary>
        public string PathAndQuery
        {
            get {
                return
                this.RequestLine.URI.Contains("://") && Uri.IsWellFormedUriString(this.RequestLine.URI, UriKind.Absolute) ? new Uri(this.RequestLine.URI).PathAndQuery
                    : this.RequestLine.URI.Contains("/") ? this.RequestLine.URI
                    : string.Empty;
            }
        }

        /// <summary>
        /// リクエストの文字エンコーディング。
        /// content-typeヘッダに指定されたcharsetを元に生成される。
        /// 指定がない場合はUS-ASCII。
        /// </summary>
        public Encoding Charset {get{return this.Headers.GetEncoding();}}

        /// <summary>
        /// HTTPリクエストボディを文字列で取得する。
        /// Transfer-Encoding: chunked なHTTPリクエストの RequestBody の読み取りは未対応。
        /// </summary>
        public string BodyAsString {
            get
            {
                if (this.Body != null)
                {
                    return this.Body.ToString(this.Charset);
                }
                return null;
            }
        }
        public override string ToString()
        {
            return 
            string.Format("{0}{1}", this.RequestLine, Environment.NewLine) +
               string.Format("{0}{1}", this.Headers.HeadersInOrder, Environment.NewLine) +
               string.Format("{0}{1})", this.BodyAsString, Environment.NewLine);
        }
    }
}
