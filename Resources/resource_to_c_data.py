import os,sys
import mimetypes

#print("Start")

LINE_WIDTH = 16
PATH_NAME_PAD_WIDTH = 4
ENCODING_NAME = "ASCII"

accept_file_types = set(['html', 'htm', 'ssi', 'gif', 'js', 'ico'])

mimetypes.add_type('text/html', '.ssi')

def print_usage():
    print("%s %s [The resource name] [http response number]" % (sys.executable, __file__))
    pass
    
if __name__ == '__main__':  
    if(3 != len(sys.argv)):
        print("incorrect parameter!")
        print_usage()
        exit()

    # First parameter: input resource name
    input_name = sys.argv[1]
    relative_path_name = "/%s" % input_name
    input_name_split_array = input_name.split('.')
    if(input_name_split_array[-1] not in accept_file_types):
        print("unsupported resource type")
        exit()
    
    # Second parameter, input resource response number
    http_response_num = int(sys.argv[2])
    
    c_array_name = "data__"
    for i in range(len(input_name_split_array) - 1):
        c_array_name += input_name_split_array[i]
        c_array_name += '_'
    c_array_name += input_name_split_array[-1]

    offset_def_name = 'PAD_'
    for i in range(len(input_name_split_array)):
        offset_def_name += input_name_split_array[i].upper()
        offset_def_name += '_'
    offset_def_name += "DATA_OFFSET"
    
    output_file_name = c_array_name + ".h"
    
    # Read the original resource content
    fin = open(input_name, mode="rb")
    content = fin.read()
    fin.close()

    fout = open(output_file_name, mode="wb")
    
    array_begin_str = "static const uint8_t %s[] = {\r\n" % c_array_name
    array_end_str = "\r\n};\r\n"
    #print("input:%s, len:%d" % (input_name, len(content)))

    #print("generate header...")

    # Generate comment
    comment_str = "This file is generated by script. Don't modify it manually.\r\n" + \
    "usage:\r\n" + \
    "%s %s [The resource name] [http response number]\r\n" % (sys.executable, __file__) + \
    "script author: zhanzr@foxmail.com\r\n"
    
    fout.write(bytes("/*\r\n", encoding=ENCODING_NAME))
    fout.write(bytes(comment_str, encoding=ENCODING_NAME))
    fout.write(bytes("\r\n*/\r\n", encoding=ENCODING_NAME))

    fout.write(bytes(array_begin_str, encoding=ENCODING_NAME))
    
    #print("/*\r\n")
    #print(relative_path_name)
    #print("\r\n*/\r\n")
    
    fout.write(bytes("/*\r\n", encoding=ENCODING_NAME))
    fout.write(bytes(relative_path_name, encoding=ENCODING_NAME))
    fout.write(bytes("\r\n*/\r\n", encoding=ENCODING_NAME))

    byte_cnt = 0
    for c in relative_path_name:
        fout.write(bytes(("0x%02X, " % ord(c)), encoding=ENCODING_NAME))
        byte_cnt += 1
        if(0 == (byte_cnt%LINE_WIDTH)):
            fout.write(bytes("\r\n", encoding=ENCODING_NAME))
            
    original_relative_path_name_len = len(relative_path_name)
    # At least one
    to_pad_len = PATH_NAME_PAD_WIDTH - (original_relative_path_name_len % PATH_NAME_PAD_WIDTH)
    padded__len = original_relative_path_name_len + to_pad_len
    for i in range(to_pad_len):
        fout.write(bytes(("0x00, "), encoding=ENCODING_NAME))
    fout.write(bytes("\r\n", encoding=ENCODING_NAME))

    fout.write(bytes("/*\r\n", encoding=ENCODING_NAME))
    
    html_reply_str = ""
    content_len_str = ""
    mime_str = ""
    
    if(200 == http_response_num):
        html_reply_str = "HTTP/1.0 200 OK\r\n" + \
        "Server: lwIP/2.1.0 (http://savannah.nongnu.org/projects/lwip)\r\n" + \
        "Content-Length: "

        content_len_str = "%d" % len(content)

        mime_str ="\r\n" + \
        "Content-Type: " + \
        mimetypes.guess_type(input_name)[0] + \
        "\r\n\r\n"
    elif(404 == http_response_num):
        html_reply_str = "HTTP/1.0 404 File not found\r\n" + \
        "Server: lwIP/2.1.0 (http://savannah.nongnu.org/projects/lwip)\r\n" + \
        "Content-Length: "

        content_len_str = "%d" % len(content)

        mime_str ="\r\n" + \
        "Content-Type: text/html\r\n\r\n"
    else:
        print("Unsupported http response number: %d" % http_response_num)
        exit()
        
    html_header_str = html_reply_str + content_len_str + mime_str
    
    fout.write(bytes(html_header_str, encoding=ENCODING_NAME))
    fout.write(bytes("\r\n*/\r\n", encoding=ENCODING_NAME))

    byte_cnt = 0
    for c in html_header_str:
        fout.write(bytes(("0x%02X, " % ord(c)), encoding=ENCODING_NAME))
        byte_cnt += 1
        if(0 == (byte_cnt%LINE_WIDTH)):
            fout.write(bytes("\r\n", encoding=ENCODING_NAME))
        
    fout.write(bytes("\r\n", encoding=ENCODING_NAME))  
    fout.write(bytes("/*\r\n", encoding=ENCODING_NAME))
    fout.write(bytes("The Content of the page", encoding=ENCODING_NAME))  
    fout.write(bytes("\r\n*/\r\n", encoding=ENCODING_NAME))

    byte_cnt = 0
    for c in content:
        fout.write(bytes(("0x%02X, " % c), encoding=ENCODING_NAME))
        byte_cnt += 1
        if(0 == (byte_cnt%LINE_WIDTH)):
            fout.write(bytes("\r\n", encoding=ENCODING_NAME))
        
    fout.write(bytes("\r\n", encoding=ENCODING_NAME)) 

    fout.write(bytes(array_end_str, encoding=ENCODING_NAME))

    fout.write(bytes("\r\n", encoding=ENCODING_NAME))  
    fout.write(bytes(("#define %s %d" % (offset_def_name, padded__len)), encoding=ENCODING_NAME))
    fout.write(bytes("\r\n", encoding=ENCODING_NAME))  

    fout.close()
    
    #print("generate header over")
