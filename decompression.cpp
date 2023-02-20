// Code by Reyaan Jagnani (reyaan44)
#include<bits/stdc++.h>
#define endl "\n"
#define quick ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)
using namespace std;

void set_file_name(string original, string &name)
{
    int n = original.size();
    for(int i=0; i<n; i++)
    {
        if(original[i]=='-')
            break;
        name.push_back(original[i]);
    }
}

string dec_to_bin(unsigned char n)
{
    string ans = "";
    while(n>0)
    {
        if(n&1)
            ans.push_back('1');
        else
            ans.push_back('0');
        n>>=1;
    }

    while(ans.size()<8)
        ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

int main(int argc, char *argv[])
{
    quick;

    // opening the input file using command line argument
    FILE *input_file = fopen(argv[1], "rb");
    if(input_file == NULL)
    {
        cout<<"Input File Could Not be Opened"<<endl;
        return 0;
    }
    else
    {
        string input_file_name = "";
        // setting the input file name
        set_file_name(argv[1], input_file_name);
        // this will be the name of the decompressed file
        string output_argument = input_file_name + "-reyaan_decompressed_it.";

        // finding the file type
        int input_file_name_sz = 0;
        char buff[1];
        // read the size
        fread(buff, 1, 1, input_file);
        input_file_name_sz = buff[0] - '0';
        for(int i=0; i<input_file_name_sz; i++)
        {
            fread(buff, 1, 1, input_file);
            output_argument.push_back(buff[0]);
        }

        FILE *output_file = fopen(output_argument.c_str(), "w");
        if(output_file == NULL)
        {
            cout<<"Output File Could Not be Opened"<<endl;
            return 0;
        }

        /*
        Now, we have codes in the program, so, we have to 
        traverse till there are two consecutive NULL
        characters
        */
        unordered_map<string,char> decode_map;
        int null = 1;
        string codes_mapping = "";
        while(null<2)
        {
            fread(buff, 1, 1, input_file);
            if(buff[0]=='\0')
            {
                if(codes_mapping.size()>0)
                    decode_map[codes_mapping.substr(1)] = codes_mapping[0];
                codes_mapping = "";
                null++;
            }
            else
            {
                codes_mapping.push_back(buff[0]);
                null = 0;
            }
        }

        // now, padding
        fread(buff, 1, 1, input_file);
        int padding = buff[0] - '0';
        fread(buff, 1, 1, input_file);

        // now, the main file
        string s = "";
        while(fread(buff, 1, 1, input_file))
        {
            string num = dec_to_bin(buff[0]);
            s += num;
        }

        while(padding-->0)
            s.pop_back();

        string temp = "";
        for(int i=0; i<s.size(); i++)
        {
            temp.push_back(s[i]);
            if(decode_map.find(temp)!=decode_map.end())
            {
                fwrite(&decode_map[temp], 1, 1, output_file);
                temp = "";
            }
        }
        
        fclose(input_file);
        fclose(output_file);

        cout<<"File Decompressed"<<endl;

    }
    return 0;
}
/*
File Type : 

3txt (3 is the size of the type, and txt is the type of file)
a0011NULL (a is character, 0011 is variable length code, NULL is to know that one character has ended)
b0101NULL
c0011NULL
NULL      (NULL if appears twice, we know the codes are ended)
padding   (Padding)
NULL      (Null to know Padding is over)
000000000 11111111 00000000 11111111 (The data)
*/