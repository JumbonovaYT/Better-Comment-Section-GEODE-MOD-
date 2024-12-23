#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>
#include <regex>
class $modify(CommentCell) {
	void loadFromComment(GJComment* comment) {
		auto logActions = Mod::get()->getSettingValue<bool>("Log actions");

		bool isHidden = false;
		auto spambutton = this->getChildByIDRecursive("spam-button"); // this is why the mod requires node ids 1.14.1
		if (spambutton) isHidden = true;
		
		/*Hide Bot Comments*/
		if (Mod::get()->getSettingValue<bool>("Hide bot comments")) {
			auto commenter = comment->m_userID;
			if (commenter == 6412437 /*DevilCru is a ja'kalem bot*/) { // i think the bots in v1.1.0 had their comments deleted
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) log::debug("found bot comment");
					CommentCell::loadFromComment(comment);
					return;
				} else {
					if (logActions) log::debug("opened bot comment");
					comment->m_isSpam = false;
					isHidden = false;
					CommentCell::loadFromComment(comment);
					return;
				}
			}
		}
		std::string commentstring = comment->m_commentString;
		std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower); // this is for brainrot, ads, and link comments. moved it here to use less lines

		/*Hide Brainrot*/
		if (Mod::get()->getSettingValue<bool>("Hide brainrot comments")) {
			std::regex brainrotWords("skibi(d|t)i|rizz|ohio|gya(t|a)|mew( +|ing|er)|mog|jelq|goon|sigma|livvy *dunne|baby *gronk|gri(tt|dd)y|fanum *tax|edg(e|ing|er)|(1 *2)? *buckle *my *shoe|huzz|h*a*w*k* *t+u+a+h+"); // WHY ARE THERE SO MANY BRAINROT WORDSSSSS
			if (std::regex_search(commentstring, brainrotWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) log::debug("found brainrot comment");
					CommentCell::loadFromComment(comment);
					return;
				} else {
					if (logActions) log::debug("opened brainrot comment");
					isHidden = false;
					comment->m_isSpam = false;
					CommentCell::loadFromComment(comment);
					return;
				}
			}
		}

		/*Hide Ads*/
		if (Mod::get()->getSettingValue<bool>("Hide ads")) {
			std::regex adWords("(check *(out)?|play|try|test) *\\d|(check *(out)?|play|try|test) *(my|this|me|the) *((recent)?|(new)?|(latest)?) *(est|er)* *(level|lvl)|(click|tap|press|view|check) *(on *my|my) *(profile|pfp|icon|name|user)"); // sorry theres kinda a lot
			if (std::regex_search(commentstring, adWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) log::debug("found ad comment");
					CommentCell::loadFromComment(comment);
					return;
				} else {
					if (logActions) log::debug("opened ad comment");
					comment->m_isSpam = false;
					isHidden = false;
					CommentCell::loadFromComment(comment);
					return;
				}
			}
		}

		/*Hide Links*/
		if (Mod::get()->getSettingValue<bool>("Hide links")) {
			std::regex linkWords("https*|(web|www)( *dot|\\.)|(\\.|dot *)gg|(\\.|dot) *com|(\\.|dot) *io|(\\.|dot) *org");
			if (std::regex_search(commentstring, linkWords)) {
				if (!isHidden) {
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) log::debug("found comment with a link");
					CommentCell::loadFromComment(comment);
					return;
				} else {
					if (logActions) log::debug("opened comment with a link");
					comment->m_isSpam = false;
					isHidden = false;
					CommentCell::loadFromComment(comment);
					return;
				}
			}
		}
	CommentCell::loadFromComment(comment);
	}
};
// i added all the"CommentCell::loadFromComment(comment)" lines because when blocked bots say a blocked keyword, the comment cant be opened, and sometimes they dont even get blocked
