using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nekoxy
{
    /// <summary>
    /// HTTPセッションデータ。
    /// </summary>
    public class Session
    {
        /// <summary>
        /// HTTPリクエストデータ。
        /// </summary>
        public HttpRequest Request { get; internal set; }

        /// <summary>
        /// HTTPレスポンスデータ。
        /// </summary>
        public HttpResponse Response { get; internal set; }

        public override string ToString()
        {
            return string.Format("{0}{1}{1}", this.Response, Environment.NewLine) + string.Format("{0}", this.Response);
            /*
            return $"{this.Request}{Environment.NewLine}{Environment.NewLine}" +
               $"{this.Response}";
             * */
        }
    }
}
