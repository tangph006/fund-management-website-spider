using System;
using System.Text.RegularExpressions;

namespace FM.Util
{
    /// <summary>
    /// Summary description for RegExUtil.
    /// </summary>
    public class RegExUtil
    {
        #region fields
        private static Regex[] m_standardRegularExpressions;
        #endregion


        /// <summary> Class cannot be instantiated</summary>
        private RegExUtil()
        {
        }

        /// <summary> 
        /// Get a predefined regular expression
        /// </summary>
        /// <param name="regularExpressionId">Id of the regular expression to return
        /// </param>
        /// <returns>RegEx</returns>
        public static Regex GetRegEx(RegularExpression regularExpressionId)
        {
            if (m_standardRegularExpressions == null)
            {
                m_standardRegularExpressions =
                   new Regex[Enum.GetNames(typeof(RegularExpression)).Length];
            }

            int index = (int)regularExpressionId;

            if (m_standardRegularExpressions[index] == null)
            {
                m_standardRegularExpressions[index] =
                   StandardRegularExpression(regularExpressionId);
            }

            return m_standardRegularExpressions[index];
        }

        /// <summary> 
        /// Get a match object based on a predefined regular expression
        /// </summary>
        /// <param name="regularExpressionId">Id of the regular expression to return</param>
        /// <param name="text">Text to match on</param>
        /// <returns>Match</returns>
        public static Match GetMatchRegEx(RegularExpression regularExpressionId, string text)
        {
            return GetRegEx(regularExpressionId).Match(text);
        }

        private static Regex StandardRegularExpression(RegularExpression regularExpressionId)
        {

            switch (regularExpressionId)
            {
                case RegularExpression.UrlExtractor:
                    {  // Refer to http://www.standardio.org/article.aspx?id=173 for help
                        return new Regex(
                           @"(?:href\s*=)(?:[\s""']*)(?!#|mailto|location.|javascript|.*css|.*this\.)(?<url>.*?)(?:[\s>""'])"
                           , RegexOptions.IgnoreCase);
                    }
                case RegularExpression.SrcExtractor:
                    {
                        return new Regex(
                           @"(?:src\s*=)(?:[\s""']*)(?<url>.*?)(?:[\s>""'])"
                           , RegexOptions.IgnoreCase);
                    }
            }

            return null;
        }

    }

    public enum RegularExpression
    {
        UrlExtractor,
        SrcExtractor
    }

}
