#include "../Header/TextProcessor.h"

// Splits line by a delimiter i.e whole string into a storage container
// void TextProcessor::splitLine(stringstream &ss, char dl){
//   for (string item; getline(ss, item, dl); ){
//     //storage.push_back(item);
//     storage.push(item);
//   }
// }

// // Trim whitespace at both ends of a string
// void TextProcessor::trimString(string &str){
//   int p1, p2;
//   p1 = p2 = 0;
//   for (int i = 0; i < str.length(); i++){
//     if (p1 == p2 && isspace(str[i])) p1 = i;
//     else if (isspace(str[i])) p2 = i;
//   }
//   str.erase(str
// }

// // Build string from char array from start to end
// void TextProcessor::buildString(int start, int end, char *arr){
//   string str;
//   for ( ; start < end; start++){
//     ss << arr[start];
//   }
//   string str = ss.str();
//   //  trimString(str);
//   storage.push(str);
//   ss.str(string()); // Clear stringstream
//   ss.clear();
// }

// Returns a queue with strings and spaces in between each string
queue<string> TextProcessor::split(char *arr, char delim){
  queue<string> buff;
  string str;
  for (int i = 0; arr[i] != '\0'; i++){

    if (isspace(arr[i])){
      buff.push(str);
      str = "";
      str.clear();
      buff.push(string(" "));
    }
    else{
      str += arr[i];
    }

    if (arr[i+1] == '\0'){
      buff.push(str);
      str = "";
      str.clear();
    }
  }
  return buff;
}

// Clean up the buffer for printing
char* TextProcessor::formatBuffer(char *fb, int fbLength){
  // TODO encapsulate int length and initialization
  int cblength = 0;

  for (int i = 0; i < fbLength; i++){
    if (isprint(fb[i])) cblength++; // get number of printable chars
  }

  char *cleanBuffer = new char[cblength + 1];

  int k = 0;
  for (int i = 0; i < fbLength; i++){
    if (isprint(fb[i])){
      cleanBuffer[k] =  fb[i]; // put printable chars in a clean buffer
      k++;
    }
  }
  cleanBuffer[cblength + 1] = '\0';
  return cleanBuffer;
}

TextProcessor::TextProcessor(){
}

TextProcessor::TextProcessor(const char *file){
  loadFile(file);
}

TextProcessor::~TextProcessor(){
  delete[] buffer;
}

void TextProcessor::loadFile(const char *file){
  ifstream infile(file, ifstream::in | ifstream::binary); // read file
  //stringstream ss;

  if (infile.is_open()){
    //TODO Convert file text to a common text (unicode or something); unique font = bad
    //TODO handle Japanese/Chinese/more text/input?
    // for (string str; getline(infile, str); ){
    //   // NOTE maybe better to put strings on the main() stack instead of heap?
    //   char *pStr = new char[str.length() + 1];
    //   strcpy(pStr, str.c_str());
    //   ss << str;
    // }
    infile.seekg(0, infile.end);
    int rawLength = infile.tellg(); // total number of characters in file
    infile.seekg(0, infile.beg);

    char *rawBuffer = new char[rawLength];
    infile.read(rawBuffer, rawLength); // read raw data as character block
    buffer = formatBuffer(rawBuffer, rawLength);
    delete[] rawBuffer;
  }
  storage = split(buffer, ' ');
  // delim = ' ';
  // int p1 = 0;
  // for (int p2 = 0; p2 < length; p2++){
  //   if (buffer[p2] = delim){
  //     buildString(p1, p2, buffer, ss);
  //     p1 = p2;
  //   }
  // }

  //splitLine(ss, delim); // store words in vector
  infile.close();
  //  ss.str(string());
  //ss.clear();
}

char* TextProcessor::getBuffer(){
  return buffer;
}

void TextProcessor::printAll(){
  for (int i = 0; buffer[i] != '\0'; i++){
    cout << buffer[i];
  }
  cout << endl;

  for (int i = 0; i < storage.size(); i++){
    cout << storage.front();
    storage.push(storage.front());
    storage.pop();
  }
  cout << endl;

  //for (string str : storage) cout << str << endl;
  // cout << storage.size() << endl;
  // while (!storage.empty()){
  //   cout << storage.front() << endl;
  //   storage.pop();
  // }
}
