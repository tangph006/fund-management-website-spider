using System;

namespace FM.Util
{
    /// <summary>
    /// Summary description for Is.
    /// </summary>
    public class Is
    {
        /// <summary> Class cannot be instantiated</summary>
        private Is()
        {
        }

        /// <summary> Empty String tests a String to see if it is null or empty.
        /// </summary>
        /// <param name="str">String to be tested.
        /// </param>
        /// <returns> boolean true if empty.
        /// </returns>
        public static bool EmptyString(String str)
        {
            return (str == null || str.Trim().Length == 0);
        }

        /// <summary> Empty HttpCookie tests a HttpCookie to see if it is null or empty.
        /// </summary>
        /// <param name="HttpCookie">HttpCookie to be tested.
        /// </param>
        /// <returns> boolean true if empty.
        /// </returns>
        public static bool EmptyHttpCookie(System.Web.HttpCookie httpCookie)
        {
            return (httpCookie == null || httpCookie.Value == null || httpCookie.Value.Trim().Length == 0);
        }

    }
}
