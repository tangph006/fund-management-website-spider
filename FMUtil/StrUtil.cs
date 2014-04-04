using System;

namespace FM.Util
{
    /// <summary>
    /// Summary description for StrUtil.
    /// </summary>
    public class StrUtil
    {
        /// <summary> Class cannot be instantiated</summary>
        private StrUtil()
        {
        }

        #region Left
        /// <summary>
        /// Gets the leftmost n characters of a String. If n characters are not
        /// available, or the String is <code>null</code>, the String will be
        /// returned without an exception.
        /// </summary>
        /// <param name="str">The String to get the leftmost characters from</param>
        /// <param name="int">The length of the required String</param>
        /// <returns>The leftmost characters</returns>
        public static string Left(string str, int len)
        {
            if (len < 0 || str == null || str.Length <= len)
            {
                return str;
            }
            else
            {
                return str.Substring(0, len);
            }
        }
        #endregion

        #region LeftIndexOf
        /// <summary>
        /// Gets the left most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the left most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <returns>The left most characters</returns>
        public static string LeftIndexOf(string str, string search)
        {
            return LeftIndexOf(str, search, 0, false);
        }

        /// <summary>
        /// Gets the left most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the left most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="includeSearchString">If true will include the search string in the result, if false then the search string is excluded</param>
        /// <returns>The left most characters</returns>
        public static string LeftIndexOf(string str, string search, bool includeSearchString)
        {
            return LeftIndexOf(str, search, 0, includeSearchString);
        }

        /// <summary>
        /// Gets the left most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the left most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="fromIndex">The index to start the search from</param>
        /// <returns>The left most characters</returns>
        public static string LeftIndexOf(string str, string search, int fromIndex)
        {
            return LeftIndexOf(str, search, fromIndex, false);
        }

        /// <summary>
        /// Gets the left most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the left most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="fromIndex">The index to start the search from</param>
        /// <param name="includeSearchString">If true will include the search string in the result, if false then the search string is excluded</param>
        /// <returns>The left most characters</returns>
        public static string LeftIndexOf(string str, string search, int fromIndex, bool includeSearchString)
        {
            if (Is.EmptyString(str) || Is.EmptyString(search))
            {
                return str;
            }
            else
            {
                if (fromIndex > str.Length)
                {
                    return str;
                }

                int pos = str.IndexOf(search, fromIndex);

                if (pos == -1)
                {
                    return str;
                }
                else
                {
                    return str.Substring(0, pos + (includeSearchString ? search.Length : 0));
                }
            }
        }
        #endregion

        #region LeftOf
        /// <summary>
        /// Gets the characters to the left of <var>pos</var>.
        /// </summary>
        /// <param name="string">The String to get the left most characters from</param>
        /// <param name="int">The position to cut from</param>
        /// <returns>The left most characters</returns>
        public static string LeftOf(string str, int pos)
        {
            if (str == null)
            {
                return null;
            }
            else
            {
                if (pos == -1) return "";
                if (pos > str.Length) return str;

                return Substring(str, 0, pos);
            }
        }
        #endregion

        #region Right
        /// <summary>
        /// Gets the right most n characters of a String. If n characters are not
        /// available, or the String is <code>null</code>, the String will be
        /// returned without an exception.
        /// </summary>
        /// <param name="str">The String to get the right most characters from</param>
        /// <param name="int">The length of the required String</param>
        /// <returns>The right most characters</returns>
        public static string Right(string str, int len)
        {
            if (len < 0 || str == null || str.Length <= len)
            {
                return str;
            }
            else
            {
                return str.Substring(str.Length - len);
            }
        }
        #endregion

        #region RightIndexOf
        /// <summary>
        /// Gets the right most characters starting from the index of <var>search</var>.
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the right most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <returns>The right most characters</returns>
        public static string RightIndexOf(string str, string search)
        {
            return RightIndexOf(str, search, 0, false);
        }

        /// <summary>
        /// Gets the right most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the right most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="includeSearchString">If true will include the search string in the result, if false then the search string is excluded</param>
        /// <returns>The right most characters</returns>
        public static string RightIndexOf(string str, string search, bool includeSearchString)
        {
            return RightIndexOf(str, search, 0, includeSearchString);
        }

        /// <summary>
        /// Gets the right most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the right most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="fromIndex">The index to start the search from</param>
        /// <returns>The right most characters</returns>
        public static string RightIndexOf(string str, string search, int fromIndex)
        {
            return RightIndexOf(str, search, fromIndex, false);
        }

        /// <summary>
        /// Gets the right most characters starting from the index of <var>search</var>
        /// <br/>
        /// If <var>str</var> is <code>null</code>, then <var>str</var> will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the right most characters off</param>
        /// <param name="search">The string to search for</param>
        /// <param name="fromIndex">The index to start the search from</param>
        /// <param name="includeSearchString">If true will include the search string in the result, if false then the search string is excluded</param>
        /// <returns>The right most characters</returns>
        public static string RightIndexOf(string str, string search, int fromIndex, bool includeSearchString)
        {
            if (Is.EmptyString(str) || Is.EmptyString(search) || fromIndex > str.Length)
            {
                return str;
            }
            else
            {
                int pos = str.IndexOf(search, fromIndex);

                if (pos == -1)
                {
                    return str;
                }
                else
                {
                    return str.Substring(pos + (includeSearchString ? 0 : search.Length));
                }
            }
        }
        #endregion

        #region RightLastIndexOf
        /// <summary>
        /// Gets the right most characters starting from the last index of <var>search</var>.
        /// <br/>
        /// If the String is <code>null</code>, the String will be returned without an exception.
        /// <br/><br/>
        /// If <var>search</var> is not found then <var>str</var> will be returned without an exception.
        /// </summary>
        /// <param name="str">The string to get the right most characters off</param>
        /// <param name="search">The last occurence of search to start from</param>
        /// <returns>The right most characters</returns>
        public static string RightLastIndexOf(string str, string search)
        {
            if ((str == null) || (search == null))
            {
                return str;
            }
            else
            {
                int pos = str.LastIndexOf(search);

                if (pos == -1)
                {
                    return str;
                }
                else
                {
                    return str.Substring(pos + search.Length);
                }
            }

        }
        #endregion

        #region RightOf
        /// <summary>
        /// Gets the characters to the right of <var>pos</var>.
        /// </summary>
        /// <param name="string">The String to get the right most characters from</param>
        /// <param name="int">The position to cut from</param>
        /// <returns>The right most characters</returns>
        public static string RightOf(string str, int pos)
        {
            if (str == null)
            {
                return null;
            }
            else
            {
                if (pos == -1)
                {
                    return str;
                }
                return Substring(str, pos + 1);
            }
        }
        #endregion

        #region Substring
        /// <summary>
        /// Gets a substring from the specified string avoiding exceptions from invalid start indexes or null strings.
        /// </summary>
        /// <param name="str">The String to get the substring from</param>
        /// <param name="int">The index of the start of the substring</param>
        /// <returns>substring from start position</returns>
        public static string Substring(string str, int start)
        {
            if (str == null) return null;

            if (start < 0) start = 0;                   // Start must be 0 or greater
            if (start > str.Length) return "";         // Start to big

            return str.Substring(start);
        }

        /// <summary>
        /// Gets a substring from the specified string avoiding exceptions from invalid start indexes, lengths or null strings.
        /// </summary>
        /// <param name="string">The String to get the substring from</param>
        /// <param name="int">The index of the start of the substring</param>
        /// <param name="int">The number of characters in the substring. </param>
        /// <returns>substring from start position for <var>length</var> characters</returns>
        public static string Substring(string str, int start, int length)
        {
            if (str == null) return null;

            if (start < 0)
            {
                start = 0;                     // Start must be 0 or greater
            }
            if (length > str.Length - start)
            {
                length = str.Length - start;
            }

            return str.Substring(start, length);
        }
        #endregion

        #region StartsWith
        /// <summary>
        /// Similer to String.StartsWith except that it is case-insensitive.
        /// </summary>
        /// <param name="str">The string to test</param>
        /// <param name="startsWith">The string to seek</param>
        /// <returns><b>true</b> if value matches the beginning of this string or is Empty; otherwise <b>false</b></returns>
        public static bool StartsWith(string str, string startsWith)
        {
            if (str == null || startsWith == null || startsWith.Length > str.Length)
            {
                return false;
            }

            // REFACTOR: Would be quicker to test each character in a loop
            return str.ToLower().StartsWith(startsWith.ToLower());
        }
        #endregion

    }
}
