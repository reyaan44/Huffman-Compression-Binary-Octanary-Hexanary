// Code by Reyaan Jagnani (reyaan44)
#include<bits/stdc++.h>
#define endl "\n"
#define quick ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)
using namespace std;

// 2 - Binary, 4 - Quadnary, 8 - Octanary, 16 - Hexanary....
const int tree = 2;
int log_sz_of_tree = 0;

void pre()
{
    int temp = tree;
    while(temp>>=1)
        ++log_sz_of_tree;
}

void set_file_name_type(string original, string &name, string &type)
{
    bool check = 1;
    int n = original.size();
    for(int i=0; i<n; i++)
    {
        if(original[i]=='.')
            check = 0;
        else
        {
            if(check)
                name.push_back(original[i]);
            else
                type.push_back(original[i]);
        }
    }    
}

void write(FILE *output, string s)
{
    int n = s.size();
    fwrite(s.c_str(), 1, n, output);
}

class node
{
public:
    node* next[tree];
    int freq;
    char ch;
    bool leaf;
    node()
    {
        for(int i=0; i<tree; i++)
            next[i] = NULL;
        freq = 0;
        leaf = 1;
    }
    node(char ch, int freq)
    {
        this->ch = ch;
        this->freq = freq;
        for(int i=0; i<tree; i++)
            next[i] = NULL;
        leaf = 1;
    }
    node(int freq, vector<node*> &vect)
    {
        this->freq = freq;
        for(int i=0; i<vect.size(); i++)
            next[i] = vect[i];
        leaf = 0;
    }
};

string dec_to_bin(int n)
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

    while(ans.size()<log_sz_of_tree)
        ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

void dfs(node* &root, string &s, unordered_map<char,string> &m)
{
    if(root->leaf)
        m[root->ch] = s;
    else
    {
        for(int i=0; i<tree; i++)
        {
            if(root->next[i]==NULL)
                continue;
            s += dec_to_bin(i);
            dfs(root->next[i], s, m);
            for(int j=0; j<log_sz_of_tree; j++)
                s.pop_back();
        }
    }
}

int main(int argc, char *argv[])
{
    quick;

    // find the toal bits that we will add in every step of the tree dfs
    pre();

    // opening the input file using command line argument
    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL)
    {
        cout<<"Input File Could Not be Opened"<<endl;
        return 0;
    }
    else
    {
        string input_file_name = "", input_file_type = "";
        // setting the input file name and type separately
        set_file_name_type(argv[1], input_file_name, input_file_type);
        // this will be the name of the compressed file
        string output_argument = input_file_name + "-reyaan_compressed_it.bin";

        FILE *output_file = fopen(output_argument.c_str(), "wb");
        if(output_file == NULL)
        {
            cout<<"Output File Could Not be Opened"<<endl;
            return 0;
        }

        /*
        Write the size of the filetype followed by the filetype
        in the compressed binary file
        */
        write(output_file, to_string(input_file_type.size())+input_file_type);

        // Take the complete data into a string
        string input_data_char = "", input_data_bin = "";
        char buff[1];

        // find the frequency of the characters
        unordered_map<char,int> frequency;
        while(fread(buff, 1, 1, input_file))
        {
            frequency[buff[0]]++;
            input_data_char.push_back(buff[0]);
        }
        // close the file to free the resources
        fclose(input_file);

        // make a huffman tree
        priority_queue<pair<int,node*>, vector<pair<int,node*>>, greater<pair<int,node*>>> min_heap;
        // new variable length codes for all characters
        unordered_map<char,string> code_to_string_mapping;
        node* root;
        for(auto itr: frequency)
        {
            node* temp = new node(itr.first, itr.second);
            // adding all the nodes in the min_heap
            min_heap.push({itr.second,temp});
        }
        while(min_heap.size()>0)
        {
            // we have found the root node
            if(min_heap.size()==1)
            {
                root = min_heap.top().second;
                min_heap.pop();
                break;
            }
            vector<node*> node_pack;
            int total_frequecies = 0;
            // taking all the children as per the tree type
            while(min_heap.size()>0 && node_pack.size()<tree)
            {
                node* curr_node = min_heap.top().second;
                int curr_freq = min_heap.top().first;
                min_heap.pop();
                total_frequecies+=curr_freq;
                node_pack.push_back(curr_node);
            }
            node* temp = new node(total_frequecies, node_pack);
            min_heap.push({total_frequecies,temp});
        }

        string s = "";
        // finding all the code string mappings
        dfs(root, s, code_to_string_mapping);

        // taking the data in the input_data_bin in binary
        for(int i=0; i<input_data_char.size(); i++)
            input_data_bin += code_to_string_mapping[input_data_char[i]];

        // padding so the size of the file during reading is divisible by 8, so we can take out 1 byte at a time
        int padding = 0;
        while((input_data_bin.size())%8)
        {
            input_data_bin.push_back('\0');
            padding++;
        }

        // write codes in input file
        for(auto itr: code_to_string_mapping)
        {
            string code = itr.first + itr.second;
            fwrite(code.c_str(), code.size()+1, 1, output_file);
            // code.size()+1 so that NULL character is in the end
        }

        // write one more Null charcter so we know codes are ended
        char null_char = '\0';
        fwrite(&null_char, 1, 1, output_file);

        // write padding
        char padding_str = padding + '0';
        fwrite(&padding_str, 1, 1, output_file);

        // null
        fwrite(&null_char, 1, 1, output_file);

        // write the whole file
        for(int i=0; i<input_data_bin.size(); i+=8)
        {
            int bit_pos = 7;
            unsigned char byte = 0b00000000;
            for(int j=i; j<i+8; j++, bit_pos--)
            {
                if(input_data_bin[j] == '1')
                    byte |= (1<<bit_pos);
            }
            buff[0] = byte;
            fwrite(buff, 1, 1, output_file);
        }

        cout<<"File Compressed"<<endl;

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