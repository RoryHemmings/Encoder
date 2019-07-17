# Encoder
Encodes between text, base64 and binary, as wells as converting images to base64 and back
Supports file input and spaces control.
# Versions
There is a c++ version (v1.2) which can be compiled into and exe,
and a python version (v1.0) which can be run with python
# Supported Conversions
  - Text to Base64
  - Base64 to Text
  - Text to Binary
  - Binary to Text
  - Image to Base64
  - Base64 to Image
# Arguments:
		Arguments:
		-h, --help: displays help menu
		-f <filepath>: feeds program whatever is in the file indicated by the path
		-o <filepath>: writes program output to file indicated by path
		-s: puts spaces in output (if converting to binary)
		
		Combine any of these two as an argument:
		i: image (can work for any file if you desire to convert raw binary data)
		t: ascii
		b: binary
		b64: base64 (newline not accepted)
		ex. <option1><option2>: converts from option1 as input to option 2 as output
		ex. encode b64t <base64text>: converts base64 input to ascii output
		ex. encode tb <ascii text> -f "input.txt": reads ascii text from "input.txt" and prints it out on the screen
		ex. encode tb <ascii text> -f "input.txt" -o "output.txt": reads ascii from "input.txt" and writes output to "output.txt"
		ex. encode ib64 <image path> -o "output.txt": reads image data from image path provided and outputs base64 into "output.txt" (Note, don't use -f for image file input)
		ex. encode b64i <b64 text> -o <output path>: writes base64 input into file specified by -o path (output.png by default)
		ex. encode b64i -f <b64input path> -o <output path>: writes base64 input into file specified by -o path with input from -f path
