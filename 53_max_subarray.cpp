/*
53. Maximum Subarray

Find the contiguous subarray within an array (containing at least one number) which has the largest sum.

For example, given the array [-2,1,-3,4,-1,2,1,-5,4],
the contiguous subarray [4,-1,2,1] has the largest sum = 6.

More practice:
If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.

*/

int maxSubArray(vector<int>& a) 
{        
    if(a.size()==0){
        return 0;
    }
    
    int sum = 0;
    int res = a[0];

    for (size_t i = 0; i < a.size(); ++i)   
    {
        sum += a[i];
        res = std::max(sum, res);
        sum = std::max(sum, 0);
    }
    
    return res;
}  
