#ifndef DATA_WAND_H
#define	DATA_WAND_H

#include "Data/Item.h"
#include <string>
#include <map>
#include <vector>

// non-directional doesn't ask for a direction
// beam asks for a direction but doesn't animate or reflect
// ray asks for a direction, animates, and can bounce or reflect
// unsure is used for random appearances
#define WAND_ZAP_TYPE_UNSURE 0
#define WAND_ZAP_TYPE_NONDIRECTIONAL 1
#define WAND_ZAP_TYPE_BEAM 2
#define WAND_ZAP_TYPE_RAY 3

namespace data {

	class Wand : public Item {
	public:
		virtual ~Wand();

		static void init();
		static const std::map<const std::string, const Wand*>& items();
		static const std::vector<std::string>& appearances();
		int maximumCharges() const;
		int zapType() const;
		const std::string& engraveMessage() const; //returns "" if no message
		bool autoidentifiesOnEngraving() const; //true if the wand auto-identifies after engraving with it

	protected:
		Wand(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, bool autoidentifies_on_engrave, unsigned long long properties);

		static void addToMap(const std::string& name, const Wand* wand);

	private:
		static std::map<const std::string, const Wand*> _wands;
		static std::vector<std::string> _wand_appearances;

		static void create(const std::string& name, int cost, int material, int maximum_charges, int zap_type, const std::string& engrave_message, bool autoidentifies_on_engrave, unsigned long long properties);

		const int _maximum_charges;
		const int _zap_type;
		const std::string _engrave_message;
		const bool _autoidentifies_on_engrave;
	};
}

#endif	/* DATA_WAND_H */
