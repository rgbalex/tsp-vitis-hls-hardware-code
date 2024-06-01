# Development of Accelerated Hardware Component to solve the Traveling Salesman Problem

https://uoy.atlassian.net/wiki/spaces/RTS/overview?homepageId=35684366

This contains many different scratches and iterations of code and ideas/approaches that I attempted to use to generate hardware to solve the TSP. 

Initially I implemented a k-opt search although due to Vitis being unhappy at most of my attempts at dynamic programming I only developed working implementations for simple 2-opt and 3-opt node swaps - not edge swaps as I now realise was important.

I then attempted to implement a simulated-annealing approach which was very successful on smaller sizes of problem (n=8 was about its limit) and I found it was extremely difficult to set the temperature and cooling rate to reasonable values. 
Once getting them set to what I thought was good values I realised that as the problem size increased that the values had to change and this made the code not particularly great at scale of sizes greater than 8 cities. 
I also found that on more complex solutions such as solution 6 in my test_bench_X.cpp that annealing would often get stuck at local minima and would not actually explore enough to find a route that was initally longer. 

I imagine with some backtracking this would be resolved but I knew Vitis had an overall aversity to recursion and therefore I was dubious to attempt this. 

If I had much more time, I would have liked to look more into ant-colony optimization as a way to fix the shortcomings of the annealing solution however I was not able to get around to it. 

## Final Solution

My final solution is a brute-force approach to the problem, and contains a slightly altered version of the quickperm algorithm (see https://quickperm.org/index.php) that aided in generating permutations of tours. 

The algorithm starts and ends at city 0 (although this is arbritrary as the distances are symmetric) and permutes the remaining cities, before adding the edge back to city 0 and calculating costs. 
The best cost path is saved and written back to shared memory, and the best cost is written back to the location given in the parameter for the hardware. 

I found this to be an effective solution as it was extremely reliable, but due to its exhaustive nature it was extremely slow on any dataset above n=13 (solving this in about 3-5 minutes). n=12 is reliably solved in 10-15 seconds for comparison and all smaller values are in a matter of milliseconds. 
Running this for longer periods does work, (admittedly I only tested this with n=14 but as this took hours to run I assumed any larger sized dataset would be out of reach for the 100mhz CPU in the FPGA) but each size above 13 takes an order of magnitude longer to complete.

In summary, the solution built and implemented is amazing for all sizes of problems smaller than n=12. Since it is a brute force approach, one advantage is that it should be easy to undo the only acheviable optimization made and modify this hardware to work on asymmetric datasets but this is outside of the scope of the project. 
