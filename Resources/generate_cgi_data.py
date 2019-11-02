import os,sys

#print("Start")

input_name = "cgi.htm"
relative_path_name = "/%s" % input_name
c_array_name = "data__cgi_htm"
offset_def_name = "CGI_DATA_OFFSET"
LINE_WIDTH = 16
PATH_NAME_PAD_WIDTH = 4

fin = open(input_name, mode="rb")

content = fin.read()

fin.close()

array_begin_str = "static const uint8_t  %s[]  = {\r\n" % c_array_name
array_end_str = "\r\n};\r\n"
#print("input:%s, len:%d" % (input_name, len(content)))

#print("generate header...")

print(array_begin_str)

print("/*")
print(relative_path_name)
print("*/")
byte_cnt = 0
for c in relative_path_name:
    print("0x%02X, " % ord(c), end="")
    byte_cnt += 1
    if(0 == (byte_cnt%LINE_WIDTH)):
        print()
        
original_relative_path_name_len = len(relative_path_name)
# At least one
to_pad_len = PATH_NAME_PAD_WIDTH - (original_relative_path_name_len % PATH_NAME_PAD_WIDTH)
padded__len = original_relative_path_name_len + to_pad_len
for i in range(to_pad_len):
    print("0x00, ", end="")
print()

print("/*")
html_reply_str = "HTTP/1.0 200 OK\r\n" + \
"Server: lwIP/2.1.0 (http://savannah.nongnu.org/projects/lwip)\r\n" + \
"Content-Length: "

content_len_str = "%d" % len(content)

mime_str ="\r\n" + \
"Content-Type: text/html\r\n"

html_header_str = html_reply_str + content_len_str + mime_str
print(html_header_str)
print("*/")

byte_cnt = 0
for c in html_header_str:
    print("0x%02X, " % ord(c), end="")
    byte_cnt += 1
    if(0 == (byte_cnt%LINE_WIDTH)):
        print()
    
print()   
print("/*")
print("The Content of the page")
print("*/")

byte_cnt = 0
for c in content:
    print("0x%02X, " % c, end="")
    byte_cnt += 1
    if(0 == (byte_cnt%LINE_WIDTH)):
        print()
    
print()   

print(array_end_str)
    
print("#define %s %d" % (offset_def_name, padded__len))
print()   

#print("generate header over")

