
#include <iostream>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

/* create following json object
{
"pi": 3.141,
"happy": true,
"name": "Niels",
"nothing": null,
"answer": {
	"everything": 42
},
"list": [1, 0, 2],
"object": {
	"currency": "USD",
	"value": 42.99
}
}
*/

int main(int argc, char const *argv[])
{
	// create an empty structure (null)
	json j1;

	// add a number that is stored as double (note the implicit conversion of j to an object)
	j1["pi"] = 3.141;

	// add a Boolean that is stored as bool
	j1["happy"] = true;

	// add a string that is stored as std::string
	j1["name"] = "Niels";

	// add another null object by passing nullptr
	j1["nothing"] = nullptr;

	// add an object inside the object
	j1["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)
	j1["list"] = { 1, 0, 2 };

	// add another object (using an initializer list of pairs)
	j1["object"] = { {"currency", "USD"}, {"value", 42.99} };

	// instead, you could also write (which looks very similar to the JSON above)
	json j2 = {
	{"pi", 3.141},
	{"happy", true},
	{"name", "Niels"},
	{"nothing", nullptr},
	{"answer", {
		{"everything", 42}
	}},
	{"list", {1, 0, 2}},
	{"object", {
		{"currency", "USD"},
		{"value", 42.99}
	}
	}
	};

	auto j3 = R"(
	{
	"pi": 3.141,
	"happy": true,
	"name": "Niels",
	"nothing": null,
	"answer": {
		"everything": 42
	},
	"list": [1, 0, 2],
	"object": {
		"currency": "USD",
		"value": 42.99
	}
	}
	)"_json;

	auto j4 = json::parse("{\"answer\":{\"everything\":42},\"happy\":true,\"list\":[1,0,2],\"name\":\"Niels\",\"nothing\":null,\"object\":{\"currency\":\"USD\",\"value\":42.99},\"pi\":3.141}");

	std::cout << "dump in different format\n";
	std::cout << "j1.dump():\n" << j1.dump() << std::endl;
	std::cout << "j1.dump(4):\n" << j1.dump(4) << std::endl;

	std::cout << "j2:\n" << j2.dump(4) << std::endl;

	std::cout << "j4:\n" << j4.dump(4) << std::endl;
	return 0;
}

