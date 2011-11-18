#pragma once

static const char standard56[] = "R0lGODlh5gEJAJEAAAAAAP////8AAP///yH5BAEAAAMALAAAAADmAQkAAAL/jCOQoexigIHyNIDnXdChM4UH1DFURp2S+kUIBrMJfKGM3ZWl+MEt2EPRgqvhLEarFS2JnaJigi5AT+at6JMZry/aNMqsiL/VyBEz+6Zdv9EKpIZbRya4dHPf0fe8to7+xtcmuFNottQTpSfB6AI11Ojx6GjhEOkBlMk2mcFZGSEGNEmFeQmlicpWmqFqWvp6itkKKyqpemPh6knJa/n52hsZuivrezucWqycGbvc/LucLJ38PG1NCizMLAq5LXs5ywUtHbpZiy3sE23Mru3Y7T1KDE6LPU5MOZpviRxcz25vni1b+AZ+A0atEytcoMSU46KuXyMfrNQxtEixIqNO/7zcaRsWMdy0ZqYW3rr4ayI/i1dO6Xv4rRzJlufE0fSmKZbMdxkr1tyo0SeuJMU22huoUldIno9Mvuxo7qROaPquSSQIlaOxj1i7ct2KEKnXrMfCVqvZ0GQ2c0+lWk0pkhRRuVTjKmuKEW7SSgX7/vy3ryzdoXz/0hMpT2xXwYzPorrqkqlYyLsgkwWYGOxOlJIyrxvZeSGyqlXRpoocFfXBf23bFT39GKo/gVMD0qrMeHZh25sRtt752zRMa5Y1o7usEqzx5fQ2fy3e3C7sWqqra+0Fmjo3TkNsthLtlAr42s7QzpTcMG3aw0u3Cl0ZXd77XA8dqvbu0qhv+tejbllnCRF49IHy3XNYAbeJfQM2lZJGuXCmYHf1XUNhhRbidGGGGhK3YXodfghiiKaJKKJaDJGY4VmOVfjMiihW4w6KMk43Y43U2IhjjjruyGOOJo7TYYs8/nhhAQA7";

static const char standard58[] = "R0lGODlhqQEJAJEAAAAAAP////8AAP///yH5BAEAAAMALAAAAACpAQkAAAL/jCOQoexigIHytIruWu2geUBQ9AESRZ2MGiXmdb5wqYqhN9KfB5qZn/rFXi5TcMFI5BQV5gTjTK6EumKV2EQmm60stMiM/kaV3HOXAak7JTRS+Siz1e6bOkep0+l4e+PtwGQW6EAY8WLIBVVYiMHIA7kYKKFIcvjIZdG4CdnpkbkiSUlSaWhpOWpKCOrTSvqauJk5y5hamopZ+wnb6fjI4+t7GqnpaXxcjKy8y2lL/Cnzyzl8SulDbSw67FiJKB3brRktzIoqy4ybW5w+Wt7eDutcus7rHKtaLp7s2So5NT3lGq13jURBsXYJEbyFqybR+taL2MFgwKrRC3XtEDd0/xYvbvRWbeKzkc1IKRTI0ZvKgAp3ZfRHUBFKhxa0nSv5b9s+f8sY+pz0Ct65n0TvmSvKsN6+bMCiMSuJa1HUnhwBmsxo9CG1iTYbyuN1Uyc4l9OkxlNnzxxUtWl5bsR27GdQV0HvoUTqVdfRvAM7glpWsQnIcOq+va1YlZ3OW37DQhR2k7Hkp5HH5huL7+zDy331FlYLGO/UuvZE88089HRStqElPj1MFyxTsk3FqaSY8yW0mq9JHqY3WLDwdP9uDYxd+m9xVDB/9wPNNSfQSE55674E0FVp1c0OFkw2E3M/s5ABmz9PFb369bPXg2QPPz768vLr249fnn76pfch9hf/D2CA7glIYIEGHohge4jBhxuA+i1TAAA7";

// base64
// http://www.adp-gmbh.ch/cpp/common/base64.html

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

static std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;
	
	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			
			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	
	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;
		
		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);
		
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	
	return ret;
}
