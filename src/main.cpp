#include <Geode/Geode.hpp>
#include <regex> 
using namespace geode::prelude;
#include <Geode/modify/CommentCell.hpp>

class $modify(CommentCell) {
    struct Fields {
        size_t m_userID;
        std::string m_commentString;
        int m_commentID;
        bool hidden = false;
    };

    void loadFromComment(GJComment* comment) {
        if (comment->m_isSpam || m_fields->hidden) {
            if (m_fields->hidden) {
                comment->m_isSpam = false;
            }
            CommentCell::loadFromComment(comment);
            return;
        } //added a little more than return; to make hidden comments openable

        m_fields->m_userID = comment->m_userID;
        m_fields->m_commentString = comment->m_commentString;
        if (Mod::get()->getSettingValue<bool>("Log actions")) m_fields->m_commentID = comment->m_commentID;

        std::transform(m_fields->m_commentString.begin(), m_fields->m_commentString.end(), m_fields->m_commentString.begin(), tolower);

        comment->m_isSpam = this->hideCommentFromUser("Hide bot comments", {
            6412437, 14781894, 5966937
        })|| this->hideCommentFromText("Hide bot comments", std::regex(
            "check(ing)? +(out)? +youtube|sub[a-z]{0,10} *(to|2)|(atch|ee|out|ver) *\\Wja\\Wkalem\\W|\\Wja\\Wkalem\\W *(is *live|on youtube|now)"
        ) // these are what bots commonly say so if i didn't put a user above, this will probably hide the comment
        ) || this->hideCommentFromText("Hide brainrot comments", std::regex(
                "skibi(d|t)i|rizz|ohio|gya(t|a)|mew( |$|ing|er)|mog( +|$|g)|jelq|goo+n|sigma|livvy *dunne|baby *gronk|gri(t+|d+)y|fanum *tax|[^a-z]edg(e|ing)|buckle *my *shoe|huz+|t+u+a+h+|who [a-z]*now|looksmax"
            )
        ) || this->hideCommentFromText("Hide ads", std::regex(
                "id *(:|=|is){0,2} *\\d{2,10}|(should)? *(play|click|try|test|see|chea?ck|look|should) *(out|at)? *((on)? *my)? *(new|recent|lat)? *(est)? *(level|lvl|map|list|\\d)|(click|tap|press|(check|look) *(out|at)?) *((on)? *my)? *(pfp|profile|name|user|icon|cube)"
            )
        ) || this->hideCommentFromText("Hide links", std::regex(
                "[^ \\.]+ *(\\.|\\W*dot\\W*)([a-z]{2,4}|[a-z]+\\/[^ \\.]+)(\\W|$)" // finds simple stuff like example.com, 123.4.5.678.io, and 💀🎺.tk
            )
        );
        CommentCell::loadFromComment(comment);
    }

    bool hideCommentFromText(const std::string_view settingKey, const std::regex& blocked) noexcept {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return false;

        std::smatch matched;
        if (std::regex_search(m_fields->m_commentString, matched, blocked)) {
            if (Mod::get()->getSettingValue<bool>("Log actions")) log::info("hid comment id #{} because setting '{}' found '{}'",
                m_fields->m_commentID,
                settingKey,
                matched.str(0)
            );
            m_fields->hidden = true;
            return true;
        }
        return false;
    }

    bool hideCommentFromUser(const std::string_view settingKey, const std::vector<size_t>& blocked) noexcept {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return false;
        
        if (std::find(blocked.begin(), blocked.end(), m_fields->m_userID) != blocked.end()) {
            if (Mod::get()->getSettingValue<bool>("Log actions")) log::info("hid comment id #{} because setting '{}' found the commenter's userID is '{}'",
                m_fields->m_commentID,
                settingKey,
                m_fields->m_userID
            );
            m_fields->hidden = true;
            return true;
        }
        return false;
    }
};
