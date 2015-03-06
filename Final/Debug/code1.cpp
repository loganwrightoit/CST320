int main()
{
    int num(0), den(0), whole(0);
    cout << "Input a numerator: ";
    cin >> num;
    cout << "Input a denominator: ";
    cin >> den;
    whole = num / den;
    
    for (int loop = 1; loop < 2; loop++)
    {
        if (num > 0)
        {
            int scopedVar = 0;
            num = num % den;
        }
        else
        {
            if (den > 0)
            {
                den = den % num;
            }
            else
            {
                loop++; 
            }
        }
        cout << whole << " " << num << " / " << den;
    }
}