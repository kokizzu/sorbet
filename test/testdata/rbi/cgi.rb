# typed: true
require 'cgi'

T.assert_type!(CGI.escape("'Stop!' said Fred"), String)
T.assert_type!(CGI.escapeElement('<BR><A HREF="url"></A>', "A", "IMG"), String)
T.assert_type!(CGI.escapeElement('<BR><A HREF="url"></A>', ["A", "IMG"]), String)
T.assert_type!(CGI.escapeHTML('Usage: foo "bar" <baz>'), String)
T.assert_type!(CGI.escape_element('<BR><A HREF="url"></A>', "A", "IMG"), String)
T.assert_type!(CGI.escape_element('<BR><A HREF="url"></A>', ["A", "IMG"]), String)
T.assert_type!(CGI.escape_html('Usage: foo "bar" <baz>'), String)
T.assert_type!(CGI.h('Usage: foo "bar" <baz>'), String)
T.assert_type!(CGI.pretty("<HTML><BODY></BODY></HTML>"), String)
T.assert_type!(CGI.rfc1123_date(Time.now), String)
T.assert_type!(CGI.unescape("%27Stop%21%27+said+Fred"), String)
T.assert_type!(CGI.unescapeElement(CGI.escapeHTML('<BR><A HREF="url"></A>'), "A", "IMG"), String)
T.assert_type!(CGI.unescapeElement(CGI.escapeHTML('<BR><A HREF="url"></A>'), ["A", "IMG"]), String)
T.assert_type!(CGI.unescapeHTML("Usage: foo &quot;bar&quot; &lt;baz&gt;"), String)
T.assert_type!(CGI.unescape_element(CGI.escape_html('<BR><A HREF="url"></A>'), "A", "IMG"), String)
T.assert_type!(CGI.unescape_element(CGI.escape_html('<BR><A HREF="url"></A>'), ["A", "IMG"]), String)
T.assert_type!(CGI.unescape_html("Usage: foo &quot;bar&quot; &lt;baz&gt;"), String)
T.assert_type!(CGI.escapeURIComponent("'Stop!' said Fred"), String)
T.assert_type!(CGI.unescapeURIComponent("%27Stop%21%27+said%20Fred"), String)
T.assert_type!(CGI.unescapeURIComponent("%27Stop%21%27+said%20Fred", "ISO-8859-1"), String)
