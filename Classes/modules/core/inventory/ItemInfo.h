#ifndef ItemInfo_h_
#define ItemInfo_h_

#include <string>

class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();

	ItemInfo(int id, int quantity);

	void initWithServerData();

	std::string getName();

	bool isCue();

	void setId(int id);

	const int & getId();

	void setQuantity(int quantity);

	int getQuantity();

	void setItemCode(std::string itemCode);

	std::string getItemCode();

	std::string getIcon();
public:
	int _id;
	int _quantity;
	int _type;
	std::string _itemCode;
	std::string _icon;
};

#endif // !ItemInfo_h_


