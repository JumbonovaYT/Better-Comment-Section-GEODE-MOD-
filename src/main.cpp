#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>
#include <regex>
class $modify(CommentCell) {
	void loadFromComment(GJComment* comment) {
		auto logActions = Mod::get()->getSettingValue<bool>("Log actions");

		bool isHidden = false;
		auto spambutton = this->getChildByIDRecursive("spam-button"); /*this is why the mod requires 1.14.1 or greater node ids (1.14.1 because it's the latest commentcell update)*/
		if (spambutton) {
			isHidden = true;
		}

		/*Hide Bot Comments*/

		if (Mod::get()->getSettingValue<bool>("Hide bot comments")) {
			auto commenter = comment->m_userID;

			if (commenter == 242664076 /*xh2c*/ || commenter == 164866826 /*3eelolbackup*/) { // i believe both of these people's comments got deleted, so i might delete them next update
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found bot comment");
					}
				} else {
					if (logActions) {
						log::debug("opened bot comment");
					}
					comment->m_isSpam = false;
					isHidden = false;
				}
			}
		}

		/*Hide Brainrot*/

		if (Mod::get()->getSettingValue<bool>("Hide brainrot comments")) {
			std::string commentstring = comment->m_commentString;
			std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower);

			std::regex brainrotWords("skibi(d|t)i|rizz|ohio|gya(t|a)|mew | mew|mog|jelq|goon|sigma"); /*this one got way shorter after trying regex, and mew uses spaces because "somewhere" has mew in it*/

			if (std::regex_search(commentstring, brainrotWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found brainrot comment");
					}
				} else {
					if (logActions) {
						log::debug("opened brainrot comment");
					}
					comment->m_isSpam = false;
					isHidden = false;
				}
			}
		}
		
		/*Hide Ads*/

		if (Mod::get()->getSettingValue<bool>("Hide ads")) {
			std::string commentstring = comment->m_commentString;
			std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower);

			std::regex adWords("(play|try) *\\d|(play|try) *(my|this|me) *((recent)?|(new)?|(latest)?) *(est|er)* *(level|lvl)|(click|tap|press|view) *(on *my|my) *(profile|pfp|icon|name)"); // sorry theres kinda a lot

			if (std::regex_search(commentstring, adWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found ad comment");
					}
				} else {
					if (logActions) {
						log::debug("opened ad comment");
					}
					comment->m_isSpam = false;
					isHidden = false;
				}
			}
		}

		/*Hide Links*/

		if (Mod::get()->getSettingValue<bool>("Hide links")) {
			std::string commentstring = comment->m_commentString;
			std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower);

			std::regex linkWords("https*|(web|www)( *dot|\\.)|(\\.|dot) *gg|(\\.|dot) *com|(\\.|dot) *io|(\\.|dot) *org");

			if (std::regex_search(commentstring, linkWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found comment with a link");
					}
				} else {
					if (logActions) {
						log::debug("opened comment with a link");
					}
					comment->m_isSpam = false;
					isHidden = false;
				}
			}
		}
	CommentCell::loadFromComment(comment);
	}
};

/* i can now say i suck at coding :woohoo:
thanks to erymanthus for telling me to learn regex */
