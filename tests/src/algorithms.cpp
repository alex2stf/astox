int centuryFromYear(int year) {
    if(year < 100){
        return 1;
    }
    int cent = ceil((year - 1) / 100);
    return cent + 1;
}

bool checkPalindrome(std::string inputString) {
    std::string copy(inputString);
    std::reverse(copy.begin(), copy.end());
    return copy == inputString;
}
