#include<iostream>
#include<utility>
#include<vector>
#include<random>
#include<iterator>
#include "randnum.h" 

using namespace std;

#define ll long long 
#define pb push_back
#define ff first
#define ss second
#define pl pair<ll, ll>
#define mp make_pair


//Random selector from a container taken from https://gist.github.com/cbsmith/5538174
template <typename RandomGenerator = std::default_random_engine>
struct random_selector
{
	//On most platforms, you probably want to use std::random_device("/dev/urandom")()
	random_selector(RandomGenerator g = RandomGenerator(std::random_device()()))
		: gen(g) {}

	template <typename Iter>
		Iter select(Iter start, Iter end) {
			std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
			std::advance(start, dis(gen));
			return start;
		}

	//convenience function
	template <typename Iter>
		Iter operator()(Iter start, Iter end) {
			return select(start, end);
		}

	//convenience function that works on anything with a sensible begin() and end(), and returns with a ref to the value type
	template <typename Container>
		auto operator()(const Container& c) -> decltype(*begin(c))& {
			return *select(begin(c), end(c));
		}

	private:
	RandomGenerator gen;
};

ll gcd(ll m, ll n)
{
	if(!(m%n))
		return n;

	return gcd(n, m%n);
}

ll power(ll x, ll y, ll m) // modular exponentiation
{
	ll ans=1;
	x%=m;
	while(y)
	{
		if(y&1)
			ans=(ans*x)%m;
		y>>=1;
		x=(x*x)%m;
	}

	return ans%m;
}

bool is_prime(ll n) //optimised school method for primality checks
{ 
    // Corner cases 
    if (n <= 1)  return false; 
    if (n <= 3)  return true; 
  
    // This is checked so that we can skip  
    // middle five numbers in below loop 
    if (n%2 == 0 || n%3 == 0) return false; 
  
    for (int i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return false; 
  
    return true; 
} 

ll mod_inv(ll a, ll m) // returns x such that ax=1modm
{ 
	ll m0 = m; 
	ll y = 0, x = 1; 

	if (m == 1) 
		return 0; 

	while (a > 1) 
	{ 
		// q is quotient 
		ll q = a / m; 
		ll t = m; 

		// m is remainder now, process same as 
		// Euclid's algo 
		m = a % m, a = t; 
		t = y; 

		// Update y and x 
		y = x - q * y; 
		x = t; 
	} 

	// Make x positive 
	if (x < 0) 
		x += m0; 

	return x; 
}

ll rand_prime() // returns a random prime 
{
	ll lower = 1;
	ll upper = 20;
	random_device rd;
    pcg rand(rd);
	uniform_int_distribution<> u(lower, upper);
	ll n;
	while(true)
	{
		n=u(rand);
		if(is_prime(n))
		return n;
	}
}

pair<pl, pl> key_gen() // returns a pair of public and private key
{
	ll p=rand_prime();
	ll q=rand_prime();

	ll n =p*q;
	ll phi=(p-1)*(q-1);

	vector<ll> v;
	for(ll i=2;i<phi;i++)
	{
		if(gcd(phi, i)==1)
			v.pb(i);
	}

	ll e = random_selector<> select(v.begin(),  v.end());

	ll d = mod_inv(e, phi);

	return mp(mp(e, n), mp(d, n));
}

ll encrypt(ll m, ll e, ll n)
{
	return power(m, e, n);
}

ll decrypt(ll c, ll d, ll n)
{
	return power(c, d, n);
}

int main()
{
	pair<pl, pl> key = key_gen();
	pl public_key = key.ff, private_key = key.ss;

	return 0;
}
