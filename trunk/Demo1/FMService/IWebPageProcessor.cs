using System;

namespace FM.Service.WebSpider
{
    /// <summary>
    /// Summary description for IWebPageProcessor.
    /// </summary>
    public interface IWebPageProcessor
    {
        bool Process(WebPageState state);

        WebPageContentDelegate contentHandler_ { get; set; }
    }

    public delegate void WebPageContentDelegate(WebPageState state);
}
