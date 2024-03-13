#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h> 
using namespace std;

struct Node // ternary tree //
{
   char val;
   Node *parent;
   Node *left;  
   Node *middle;  
   Node *right;  
};        
  
Node* makeTreeNode(char data);  
void buildTree_and_travese_everything_whatever(Node *root, vector<char> a, vector<char> path);
string FindTwoAlleles(vector<string> a);
double occur_diffenrence(vector<char> path);
void go_get_all_folks(vector<vector<string> > C, vector<char> a);

vector< vector<string> > CASE;  
vector< vector<string> > CTRL;
// 2D vectors to hold case and control text files //

vector<char> opt_pattern; // store the optimal snp pattern when hit optimal solution //
double RESULT= 0.0; // store optimal solution //


int main(){

   string caseGroup = "psor_5snp_929case.txt";
   string controlGroup = "psor_5snp_681ctrl.txt";

   ifstream in1( caseGroup.c_str() );
   ifstream in2( controlGroup.c_str() );

//===================put case/ctrl txt into two 2-dimension vector============================================//
   for ( string line; getline( in1, line );)
   {// cite: https://www.cplusplus.com/forum/beginner/264090/   
   //  put txt file into vectors separated by rows//
      stringstream ss1( line );  
      vector<string> row;
      for ( string d; ss1 >> d;) row.push_back( d );
      CASE.push_back( row );
   }
   for ( string line; getline( in2, line );)
   {
      stringstream ss2( line );  
      vector<string> row;
      for ( string d; ss2 >> d; ) row.push_back( d );
      CTRL.push_back( row );
   }
//===================put case/ctrl txt into two 2-dimension vector============================================//
   
   string types;
   Node *root = new Node(); // root node //

   for(int i =1;i<CASE.size();i++)
   {
        // get all types into a char vector like A T X C G X .. where X means null or means not taken value here//
        types = (FindTwoAlleles(CASE[i])+"X") + types;
        // when not taking allele from a snp, we use X to represent it. //
   }

   vector<char> path;
   vector<char> type(types.begin(), types.end());
   // string types is like ATXCGX...  note: X is the third leaf representing no taking alleles from the snp//

   buildTree_and_travese_everything_whatever(root, type, path); 
   // entire ternary tree is now built, traversed, and deleted//

   cout<<endl;
   cout<< "The optimal solution for " << CASE.size()-1 << " snp case/ctrl file is: "<< RESULT <<"\n"<<endl;
   cout<< "the pattern is: " <<endl;
   for(int j = 1; j<CTRL.size(); j++)
   {
       cout<< CTRL[j][0] << "\t" << opt_pattern[j-1]<<endl;
   }
   cout<<"\n";
   cout<< "Note: The X means the program doesn't take any allele at the particular SNP, which doesn't counted in pattern size as well";
   cout<<"\n\n";

   int choice=0;
   cout << "enter '1' to see all CASE ID that has this pattern, '2' for CTRL, '3' for both, other keys to end: ";
   cin >> choice;
   if(choice == 1)
   {
      cout<< "CASE: "<<endl;
      go_get_all_folks(CASE, opt_pattern);
   }
   else if(choice == 2)
   {
      cout<< "CTRL: "<<endl;
      go_get_all_folks(CTRL, opt_pattern);
   }
   else if(choice ==3)
   {
      cout<< "CASE: "<<endl;
      go_get_all_folks(CASE, opt_pattern);
      
      cout<<"\n\n";

      cout<< "CTRL: "<<endl;
      go_get_all_folks(CTRL, opt_pattern);
   }
            
   

   opt_pattern.clear();
   CASE.clear();
   CTRL.clear();
   path.clear();
   type.clear();
   return 0;
}
string FindTwoAlleles(vector<string> a)
{
   for(int i=1; i<a.size(); i++)
   {
      if(a[i][0] != a[i][1])
      {
       return a[i];
      }
   }
   return "error"; 
   //complete this function and just in case if it finds anything weird//
}
Node* makeTreeNode(char data)
{                                               
    Node *newNode = new Node();      
    newNode->val = data;                     
    return newNode;  
} 
void buildTree_and_travese_everything_whatever(Node *root, vector<char> allele, vector<char> path)
{  
   if(!allele.empty())
   {
      // as long as allele are not empty, keep building the tree //
      root->right = makeTreeNode(allele.back());
      root->right->parent = root;
      allele.pop_back();
      root->middle = makeTreeNode(allele.back());
      root->middle->parent = root;
      allele.pop_back();
      root->left = makeTreeNode(allele.back());
      root->left->parent = root;
      allele.pop_back();
   }
   else 
   {    
        // this means tree is built to lowest level, now get one single path and delete the node //
        Node *cur= new Node;
        Node *temp = new Node;
        cur = root;
        for(int i=0; i< CASE.size()-1;i++)
        { 
           // CASE.size()-1 is just number of snp in file //
            if(cur->left == NULL && cur->middle == NULL && cur->right ==NULL)
            {
               // since leaves dont have child, they first come here, give out the allele and build the path, and get deleted //
               Node *temp = new Node;   
               temp = cur;
               path.push_back(cur->val); // get leaf value to make path //
               cur = cur->parent;
               delete temp; // delete this leaf, after 3 leaves are deleted, it will delete high level nodes as well I guess? 
            }else
            {
               path.push_back(cur->val);
               cur = cur->parent;
            }
        }

        reverse(path.begin(), path.end());
        if(RESULT< occur_diffenrence(path))
        {
            // do the occurence differenct check and let RESULT be the maximum//
            opt_pattern = path;
            RESULT = occur_diffenrence(path);
        }
        path.clear();
        return;  
   }
   buildTree_and_travese_everything_whatever(root->right, allele, path);
   buildTree_and_travese_everything_whatever(root->middle, allele, path);
   buildTree_and_travese_everything_whatever(root->left, allele, path); 
}
double occur_diffenrence(vector<char> path)
{
   // this function takes a vector<char> (a branch of tree) and calculate the diference of occurence between 2 goups // 
   double diff=0.0;
   double count1 =0;
   double count2=0;
   bool found = true;
   // take one of the path, and look in case and ctrl to see how many people have this pattern. //
   for (int i = 1; i < CASE[1].size(); i++)
   {
      for (int j = 0; j < CASE.size()-1; j++)
      {
         if(CASE[j+1][i].find('N') != std::string::npos)
         {
            cout << CASE[0][i] << " has null alleles"; // detect empty value but it wont stops the program;
         }
         else if(path[j]== 'X')
         {
            continue;
         }
         else if(CASE[j+1][i].find(path[j]) == std::string::npos)
         {
            found = false;
         }
      }
      if(found)
      {
         count1++;
      }
      found = true;
   }
   diff = count1 / (CASE[1].size()-1);
   for (int i = 1; i < CTRL[1].size(); i++)
   {
      for (int j = 0; j < CTRL.size()-1; j++)
      {
         if(CTRL[j+1][i].find('N') != std::string::npos)
         {
            cout << CTRL[0][i] << " has null alleles"; // detect empty value but it wont stops the program;
         }
         else if(path[j]== 'X')
         {
            continue;
         }
         else if(CTRL[j+1][i].find(path[j]) == std::string::npos)
         {
            found = false;
         }
      }
      if(found)
      {
         count2++;
      }
      found = true;
   }
   diff -= count2 / (CTRL[1].size()-1); 
   return abs(diff);   
}
void go_get_all_folks(vector<vector<string> > C, vector<char> a)
{
   bool found = true;
   for (int i = 1; i < C[1].size(); i++)
   {
      for (int j = 0; j < C.size()-1; j++)
      {
         if(a[j]== 'X')
         {
            continue;
         }
         else if(CASE[j+1][i].find(a[j]) == std::string::npos)
         {
            found = false;
         }
      }
      if(found)
      {
         cout<< C[0][i]<<endl;
      }
      found = true;
   }
}