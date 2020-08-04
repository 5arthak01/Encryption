#include<iostream>
#include<utility>
#include<vector>
#include<random>
#include "randnum.h" 

using namespace std;

#define ll long long 


ll gcd(ll m, ll n) // Returns highest common factor of m and n
{
	if(!(m%n))
		return n;

	return gcd(n, m%n);
}

ll power(ll x, ll y, ll m) // returns x^y mod(m)
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
  
    for (ll i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return false; 
  
    return true; 
} 

ll mod_inv(ll a, ll m) // returns positive x such that ax = 1 mod(m)
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

		// m is remainder now, process same as Euclid's algo
		m = a % m, a = t; 
		t = y; 

		// Update y and x 
		y = x - q * y; 
		x = t; 
	} 

	if (x < 0) 
		x += m0; 

	return x; 
}

ll rand_prime() // returns a random prime in range [lower, upper]
{
	ll lower = 1;
	// NOTE: For best utilisation of RSA, the lower bound must be as high as possible
	ll upper = 101; 
	// NOTE: Since during key generation product of primes is 64-bit, 
	// the upper limit should always be less than 3037000499
	
	random_device rd;
    pcg rand(rd);
	uniform_int_distribution<> uniform_generator(lower, upper);
	ll n;

	//Generate random numbers till a prime
	while(true)
	{
		n=uniform_generator(rand);
		if(is_prime(n))
			return n;
	}
}

pair<pair<ll, ll>, pair<ll, ll>> key_gen() // returns an ordered pair of public and private key
{
	ll p=rand_prime();
	ll q=rand_prime();

	ll n =p*q;
	while(n<0){
		if(p<q)
			q=rand_prime();
		else
			p=rand_prime();
		n=p*q;
	}
	ll phi=(p-1)*(q-1); // Euler's totient of 'n'

	vector<ll> v; 
	// vector containing all possibilities 'e' for public key

	// The iteration may begin with a higher number
	// than 2 as appropriate to yield better keys
	for(ll i=2;i<phi;i++)
	{
		if(gcd(phi, i)==1)
			v.push_back(i);
	}

	random_device rd;
    pcg rand(rd);
	uniform_int_distribution<> u(0, v.size()-1);

	ll e = v[u(rand)];

	// find the private key corresponding to the given public key
	ll d = mod_inv(e, phi);

	return make_pair(make_pair(e, n), make_pair(d, n));
}
  
ll encrypt(ll message, ll e, ll n){ // Public key is {e, n}
	if(message<n)
		return power(message, e, n);
	cerr<<"Error: Message incompatible with key. Please choose smaller message or higher key."<<endl;
	return 2;
}

ll decrypt(ll cipher_text, ll d, ll n){ // Private key is {d, n}
	return power(cipher_text, d, n);
}

/*
Since mnemonic names are difficult to implement here, 
here is the guide to the variables.

p,q are random primes and n = p*q
phi(n) is Euler_totient(n)

e is a random number (which serves as the public key) with:
>  1<e<phi(n)
>  gcd(phi(n), e)=1 

This is to ensure that a unique private key corresponding to
'e' exists.

The private key 'd' is the multiplicative inverse of e modular n

However, 'e' and 'd' are incomplete without n
Hence, public key is {e,n} and private key is {d,n}

Multiple keys can be generated from the same p and q pair, 
in which case 'n' is known and internalised, allowing 
the public key to be {e} and private key to be {d}
*/

int main()
{
	pair<pair<ll, ll>, pair<ll, ll>> key = key_gen();
	pair<ll, ll> public_key = key.first, private_key = key.second;

	// Demonstration
	cout<<"Enter number"<<endl;
	ll message;
	cin>>message;
	ll secure_msg = encrypt(message, public_key.first, public_key.second);
	cout<<"Encrypted: "<<secure_msg<<"\nDecrypted: "<<decrypt(secure_msg, private_key.first, private_key.second)<<endl;

	return 0;
}
