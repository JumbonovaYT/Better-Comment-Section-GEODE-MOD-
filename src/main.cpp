#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>
class $modify(CommentCell) {

	void loadFromComment(GJComment* comment) {
		/*get settings that are fetched multiple times (just to make stuff easier)*/
		auto logActions = Mod::get()->getSettingValue<bool>("Log actions");

		/*make isHidden bool*/
		bool isHidden = false;

		auto spambutton = this->getChildByIDRecursive("spam-button"); /*this is why the mod requires 1.14.1 or greater node ids (1.14.1 because it's the latest commentcell update)*/

		if (spambutton) {
			isHidden = true;
		} else {
			isHidden = false;
		};

		/*Hide Bot Comments*/

		if (Mod::get()->getSettingValue<bool>("Hide bot comments")) {
			/*save user id*/
			auto commenter = comment->m_userID;
			/*if user id is a known bot id...*/
			if (commenter == 242664076 /*xh2c*/ || commenter == 164866826 /*3eelolbackup*/) {
				if (!isHidden) {
					/*...hide comment*/
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found bot comment");
					};
				} else {
					/*and this opens the comment*/
					if (logActions) {
						log::debug("opened bot comment");
					};
					comment->m_isSpam = false;
					isHidden = false;
				};
			};
		};

		/*Hide Brainrot*/

		if (Mod::get()->getSettingValue<bool>("Hide brainrot comments")) {
			/*get the comment as lowercase*/
			std::string commentstring = comment->m_commentString;
			std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower);

			/*if the mod contains a brainrot keyword...*/
			if (commentstring.find("skibidi") != std::string::npos || commentstring.find("skibiti") != std::string::npos || commentstring.find("rizz") != std::string::npos || commentstring.find("mog") != std::string::npos || commentstring.find("gyat") != std::string::npos || commentstring.find("gyaa") != std::string::npos || commentstring.find("ohio") != std::string::npos || commentstring.find("sigma") != std::string::npos || /*sorry to anyone seeing the next 2. they're brainrot so i have to block them*/ commentstring.find("jelq") != std::string::npos || commentstring.find("goon") != std::string::npos || commentstring.find("diddy") != std::string::npos || commentstring.find("mew") != std::string::npos) {
				if (!isHidden) {
					/*...hide comment*/
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found brainrot comment");
					};
				} else {
					/*and this opens the comment*/
					if (logActions) {
						log::debug("opened brainrot comment");
					};
					comment->m_isSpam = false;
					isHidden = false;
				};
			};
		};
		
		/*Hide Ads*/

		if (Mod::get()->getSettingValue<bool>("Hide ads")) {
			/*get the comment as lowercase*/
			std::string commentstring = comment->m_commentString;
			std::transform(commentstring.begin(), commentstring.end(), commentstring.begin(), tolower);

			/*if the mod contains an ad keyword...*/
			if (/*saw a comment on retray saying "pls, play 113020262 >:3" so i decided to add these next few keywords*/commentstring.find("play 1") != std::string::npos || commentstring.find("play 2") != std::string::npos || commentstring.find("play 3") != std::string::npos || commentstring.find("play 4") != std::string::npos || commentstring.find("play 5") != std::string::npos || commentstring.find("play 6") != std::string::npos || commentstring.find("play 7") != std::string::npos || commentstring.find("play 8") != std::string::npos || commentstring.find("play 9") != std::string::npos || /*okay now to some other keywords*/ commentstring.find("play this level") != std::string::npos || /*this one was added because their post could say play my level. this one could have some false positives, so i might remove this later*/ commentstring.find("click my") != std::string::npos || /*...and this one*/ commentstring.find("click on my") != std::string::npos || commentstring.find("play my level") != std::string::npos) {
				if (!isHidden) {
					/*...hide comment*/
					isHidden = true;
					comment->m_isSpam = true;
					if (logActions) {
						log::debug("found ad comment");
					};
				} else {
					/*and this opens the comment*/
					if (logActions) {
						log::debug("opened ad comment");
					};
					comment->m_isSpam = false;
					isHidden = false;
				};
			};
		};
	/*and then finally load the comment*/
	CommentCell::loadFromComment(comment);
	};
};