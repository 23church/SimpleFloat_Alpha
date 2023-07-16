#include <regex>

class sFloatA{
    //MSB is sign, next 46 bits are whole values, bottom 17 bits are decimal values
    //whole range is 0 to 70,368,744,177,663
    //decimal range is 0.00000 to 0.99999
    //positive infinity is 120000, negative infinity is -120000
    //positive NaN is 110000, negative NaN is -110000
    int64_t value;
    public:
        sFloatA(){
            value = 0;}
        ~sFloatA(){
            };
        template <typename A> sFloatA(A init){
            value = convert(init);}
        template <typename B> void operator= (B assign){
            value = convert(assign);}
        template <typename C> sFloatA operator+= (C adding){
            value = add(value, convert(adding));
            return *this;}
        template <typename D> sFloatA operator-= (D subtracting){
            value = subtract(value, convert(subtracting));
            return *this;}
        template <typename E> sFloatA operator*= (E multiplying){
            value = multiply(value, convert(multiplying));
            return *this;}
        template <typename F> sFloatA operator/= (F dividing){
            value = divide(value, convert(dividing));
            return *this;}
        template <typename G> bool operator< (G lThan){
            int64_t compare = convert(lThan);
            if(checkNaN(compare) || checkNaN(value)){
                return false;}
            if(checkInf(compare) ^ checkInf(value)){
                if(checkInf(compare)){
                    return (compare > 0);}
                else{
                    return (value < 0);}}
            return value < compare;}
        template <typename H> bool operator== (H equal){
            int64_t compare = convert(equal);
            if(checkNaN(compare) || checkNaN(value)){
                return false;}
            return value == compare;}
        template <typename I> bool operator> (I gThan){
            return gThan < *this;}
        template <typename J> bool operator<= (J lEqual){
            if(checkNaN(convert(lEqual)) || checkNaN(value)){
                return false;}
            return !(*this > lEqual);}
        template <typename K> bool operator!= (K nEqual){
            return !(*this == nEqual);}
        template <typename L> bool operator>= (L gEqual){
            if(checkNaN(convert(gEqual)) || checkNaN(value)){
                return false;}
            return !(*this < gEqual);}
        template <typename M> sFloatA operator+ (M adding){
            sFloatA answer;
            int64_t tAnswer = add(value, convert(adding));
            if(checkNaN(tAnswer)){
                answer.setNaN(tAnswer > 0);}
            else if(checkInf(tAnswer)){
                answer.setInfinity(tAnswer > 0);}
            else{
                answer.value = tAnswer;}
            return answer;}
        template <typename N> sFloatA operator- (N subtracting){
            sFloatA answer;
            int64_t tAnswer = subtract(value, convert(subtracting));
            if(checkNaN(tAnswer)){
                answer.setNaN(tAnswer > 0);}
            else if(checkInf(tAnswer)){
                answer.setInfinity(tAnswer > 0);}
            else{
                answer.value = tAnswer;}
            return answer;}
        template <typename O> sFloatA operator* (O multiplying){
            sFloatA answer;
            int64_t tAnswer = multiply(value, convert(multiplying));
            if(checkNaN(tAnswer)){
                answer.setNaN(tAnswer > 0);}
            else if(checkInf(tAnswer)){
                answer.setInfinity(tAnswer > 0);}
            else{
                answer.value = tAnswer;}
            return answer;}
        template <typename P> sFloatA operator/ (P dividing){
            sFloatA answer;
            int64_t tAnswer = divide(value, convert(dividing));
            if(checkNaN(tAnswer)){
                answer.setNaN(tAnswer > 0);}
            else if(checkInf(tAnswer)){
                answer.setInfinity(tAnswer > 0);}
            else{
                answer.value = tAnswer;}
            return answer;}
        sFloatA operator-(){
            sFloatA negated;
            if(checkNaN(this->value)){
                negated.setNaN(this->value < 0);}
            else if(checkInf(this->value)){
                negated.setInfinity(this->value < 0);}
            else{
                negated.value = subtract(0, value);}
            return negated;}
        void setInfinity(bool positive = true){
            value = (positive ? 120000 : -120000);}
        void setNaN(bool positive = true){
            value = (positive ? 110000 : -110000);}
        bool checkInf(int64_t test = 115000){
            test = ((test == 115000) ? value : test);
            return (abs(test) == 120000);}
        bool checkNaN(int64_t test = 115000){
            test = ((test == 115000) ? value : test);
            return (abs(test) == 110000);}
        std::string get(){
            if(checkInf(value)){
                return ((value > 0) ? "+Infinity" : "-Infinity");}
            else if(checkNaN(value)){
                return ((value > 0) ? "+NaN" : "-NaN");}
            else{
                int64_t whole = abs(value) >> 17, part = abs(value) & 131071;
                std::string decimal = "00000" + std::to_string(part);
                return ((value < 0) ? "-" : "") + std::to_string(whole) + "." + decimal.substr(decimal.length()-5, 5);}}
    private:
        int64_t convert(sFloatA input){
            return input.value;}
        int64_t convert(int input){
            return ((int64_t)input << 17);}
        int64_t convert(int64_t input){
            if(abs(input) > 70368744177663){
                return ((input > 0) ? 120000 : -120000);}
            else{
                return (input << 17);}}
        int64_t convert(const char * input){
            return stoi64((std::string)input);}
        int64_t convert(std::string input){
            return stoi64(input);}
        int64_t convert(float input){
            if(isnan(input)){
                return (int64_t)(signbit(input) ? -1 : 1)*110000;}
            if((abs(input) > 70368739983360.0) || isinf(input)){ //70368739983360.0 largest in range float
                return (int64_t)(signbit(input) ? -1 : 1)*120000;}
            else{
                float temp1, temp2 = modf(abs(input), &temp1);
                int64_t whole = (int64_t)temp1 << 17, part = (int64_t)trunc(temp2*100000);
                return (signbit(input) ? -1 : 1)*(whole+part);}}
        int64_t convert(double input){
            if(isnan(input)){
                return (int64_t)(signbit(input) ? -1 : 1)*110000;}
            if((abs(input) > 70368744177663.996) || isinf(input)){ //70368744177663.996 largest in range double
                return (int64_t)(signbit(input) ? -1 : 1)*120000;}
            else{
                double temp1, temp2 = modf(abs(input), &temp1);
                int64_t whole = (int64_t)temp1 << 17, part = (int64_t)trunc(temp2*100000);
                return (signbit(input) ? -1 : 1)*(whole+part);}}
        int64_t stoi64(std::string input){
            int64_t whole, part = 0;
            std::string sConvert = "";
            std::regex isDigits(R"~(^[\d.+-]+$)~"), digits(R"~(\d{1,14}(\.\d*)?)~");
            std::smatch value;
            bool negative = (input[0] == '-');
            if(std::regex_match(input, isDigits)){
                std::regex_search(input, value, digits);
                sConvert = (std::string)value[0];}
            else{
                return 110000;} //any string with invalid chars returns NaN
            std::size_t dPoint = sConvert.find(".");
            if(dPoint < sConvert.size()){
                whole = std::stoll(sConvert.substr(0, dPoint));
                part = std::stoll((sConvert + "00000").substr(dPoint+1, 5));} //ensures 5 decimal places
            else{
                whole = std::stoll(sConvert);}
            if(whole > 70368744177663){
                return (negative ? -120000 : 120000);} //underflow/overflow
            else{
                return (negative ? -1 : 1)*((whole << 17)+part);}}
        int64_t add(int64_t num1, int64_t num2){
            if(checkNaN(num1) || checkNaN(num2)){
                return 110000;}
            if(num2 < 0){
                return ((num1 > 0) ? subtract(num1, abs(num2)) : -add(abs(num1), abs(num2)));}
            else if(num1 < 0){
                return subtract(num2, abs(num1));}
            int64_t infinities = (int64_t)(checkInf(num1) ? -(num1 < 0)|1 : 0)+(checkInf(num2) ? -(num2 < 0)|1 : 0);
            if(infinities){
                return ((infinities < 0) ? -120000 : 120000);} //adding 2 identical infinities or only one infinity
            else{
                if(checkInf(num1)){ //adding 2 opposite infinities
                    return 110000;}}
            int64_t whole1 = ((num1 >> 17) << 17), whole2 = ((num2 >> 17) << 17), whole3 = whole1+whole2, part1 = num1 & 131071, part2 = num2 & 131071, part3 = part1+part2;
            if(part3 > 99999){
                whole3 += 131072;
                part3 -= 100000;}
            return whole3+part3;}
        int64_t subtract(int64_t num1, int64_t num2){
            if(checkNaN(num1) || checkNaN(num2)){
                return 110000;}
            if(num1 == num2){
                if(checkInf(num1)){
                    return 110000;} // +infinity-(+infinity) or -infinity-(-infinity)
                else{
                    return 0;}} // x-x
            if((num2 == -120000) || (num1 == 120000)){
                return 120000;}
            if((num1 == -120000) || (num2 == 120000)){
                return -120000;}
            if(num2 < 0){
                return ((num1 < 0) ? subtract(abs(num2), abs(num1)) : add(abs(num1), abs(num2)));}
            else if(num1 < 0){
                return -add(abs(num1), num2);}
            int64_t whole1 = ((num1 >> 17) << 17), whole2 = ((num2 >> 17) << 17), whole3 = whole1-whole2, part1 = num1 & 131071, part2 = num2 & 131071, part3 = part1-part2;
            if(whole3 < 0){
                whole3 = whole2-whole1;
                part3 = part2-part1;}
            if(whole3 > 0){
                if(part3 < 0){
                    whole3 -= 131072;
                    part3 = 100000+part3;}}
            else if(whole3 == 0){
                if(part3 < 0){
                    part3 = -abs(part3);}}
            return ((whole2 > whole1) ? -1 : 1)*(whole3+part3);}
        int64_t multiply(int64_t num1, int64_t num2){
            if(checkNaN(num1) || checkNaN(num2)){
                return 110000;}
            bool negative = ((num1 ^ num2) < 0); //comparing signs
            if(checkInf(num1) || checkInf(num2)){
                if((num1 == 0) || (num2 == 0)){
                    return 110000;} // infinity*0
                else{
                    return (negative ? -120000 : 120000);}} // +-infinity*x
            int64_t whole1 = abs(num1) >> 17, part1 = abs(num1) & 131071, whole2 = abs(num2) >> 17, part2 = abs(num2) & 131071, whole3, part3, result;
            result = (whole1*whole2) << 17;
            whole3 = ((whole1*part2)/100000) << 17;
            part3 = (whole1*part2)%100000;
            result = add(result, whole3+part3);
            whole3 = ((whole2*part1)/100000) << 17;
            part3 = (whole2*part1)%100000;
            result = add(result, whole3+part3);
            whole3 = ((part1*part2)/100000);
            part3 = (part1*part2)%100000;
            result = add(result, whole3+part3);
            if(result < 0){
                return (negative ? -120000 : 120000);} //underflow/overflow
            else{
                return (negative ? -result : result);}}
        int64_t divide(int64_t num1, int64_t num2){ //numerator, denominator
            if(checkNaN(num1) || checkNaN(num2)){
                return 110000;}
            if(abs(num1) == abs(num2)){
                if(checkInf(num1) || (num1 == 0)){
                    return 110000;} // infinity/infinity or 0/0
                else{
                    return ((num1 == num2) ? 131072 : -131072);}} //shortcut for equal numerator and denominator
            if(num1 == 0){
                return 0;} // 0/x
            if(num2 == 0){
                return ((num1 > 0) ? 120000 : -120000);} //cant be trying to divide by zero now, can we?
            if(checkInf(num1)){ // +-infinity/x
                return num1;}
            if(checkInf(num2)){ // x/+-infinity
                return 0;}
            bool negative = ((num1 ^ num2) < 0); //comparing signs
            int64_t mults = 0, temp1 = abs(num1), temp2 = abs(num2), result = 0;
            if((num2 < num1) && (num2 < 922337203685465151)){ //less than 7036874417766.39999 i.e. 1/10th of max
                while(multiply(temp2, 1310720) < num1){
                    temp2 = multiply(temp2, 1310720);}}
            while(multiply(add(result, 1), abs(num2)) < abs(num1)){
                while(temp1 >= temp2){
                    temp1 = subtract(temp1, temp2);
                    mults++;}
                result = add(multiply(result, 1310720), mults);
                mults = 0;
                if(temp1 < 922337203685465151){
                    temp1 = multiply(temp1, 1310720);}
                else{
                    temp2 = multiply(temp2, 10000);}} //if temp1 is too large to be multiplied, divide temp2 instead
            return (negative ? -result : result);}};