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
            if (m_fields->hidden) comment->m_isSpam = false;
            CommentCell::loadFromComment(comment);
            return;
        }
        
        m_fields->m_userID = comment->m_userID;
        m_fields->m_commentString = comment->m_commentString;
        if (Mod::get()->getSettingValue<bool>("Log actions")) m_fields->m_commentID = comment->m_commentID;

        std::transform(m_fields->m_commentString.begin(), m_fields->m_commentString.end(), m_fields->m_commentString.begin(), tolower);

        comment->m_isSpam = this->hideCommentFromUser("Hide bot comments", {
            6412437, 14781894, 5966937, 217757207, 58344078, 252065435, 281052270, 281045271, 281651597, 18165112, 15798843, 7348073, 8537037, 157923469
        })|| this->hideCommentFromText("Hide bot comments", std::regex(
            "(atch|ee|out|ver) ?\\Wja\\Wkalem\\W|\\Wja\\Wkalem\\W ?(is ?live|on youtube|now)|^qns was here$|nice level! i liked yours, now like mine :3|read (the )?(most recent post on)? my profile( post)?" // the last one is from the recent tab bots, dunno if i should put it in ads or bots
        ) 
        ) || this->hideCommentFromText("Hide brainrot comments", std::regex(
                "skibi[dt]i|rizz|ohio|gya[ta]|m(ew|og)( |$|g?ing|g?er)|jelq|goon|sigma|([^a-z]|^|baby|livvy)(gronk|dunne)|gri(t{1,3}|d{1,3})y|fanum ?tax|[^a-z]edg(ed|ing)|buckle ?my ?shoe|huzz|tu{1,3}a{1,3}h{1,3}|those ?who ?[a-z]{0,2}now|looksmax|freak([^a-z]|$|y)|diddy|(^|[^a-z])((ts ?)?pmo|dih)([^a-z]|$)|i'?m?.{0,5} ?(am)? ?ste{1,10}ve|chicken ?jocke|water ?bucket ?release|i ?yearn(ed)? ?for ?the ?mine|(love|luv) ?crushing? ?(the|a)? ?loaf|this ?is ?a ?crafting? ?table|flint ?a?nd? ? stee"
            )
        ) || this->hideCommentFromText("Hide ads", std::regex(
                "^\\d{3,10}$|( |^)((tr(y|ied)|see|go|attempt|build?t?|rate?|beat)(ing)?|test|enjoy|cilck|chea?c?k|look|plays?|(i|me) ?(upload|create?|made)(ed)? ?a?) ?(out|at)? ?(on)? (to)? ?(m(y|eh?)|this)? ?(new|recent|lat|first)?(est|er)? ?(le?ve?l?s?|lists?|map|deco|layout|pre ?v|\\W{0,2}\\d{3,10})( ?\\d{3,10})?|(maps?|le?ve?l?s?|ids?) ?(:|=|is|are){0,2} ?[^a-z0-9]{0,2} ?(\\d{3,10}|[a-z0-9 ]{1,20} by ?[a-z0-9_]{5,20})|(click|tap{1,3}|press|see|like?|go|chea?c?k|look)(ing)? ?(out|[a-z]{2})? ?(on)? ?(to)? ?(m(y|eh?))? ?(pfp|profile|name|user|icon|cube|bio|channel|yt|you ?tube|(button|yt|youtube) ?(top|right)|(new|recent|lat|last)?(est)? ?post)|sub(scribe?(ing)?)? ?(to|2)" // sorry for how long this is, it was gonna be 1178 characters so be happy
            )
        ) || this->hideCommentFromText("Hide links", std::regex(
                "[^ \\.]{2,30}[^ \\.]?[^ a-z0-9]{0,2}(dot|\\.)[^ a-z0-9]{0,2}([a-z]{2,3}( |$)|[a-z]{1,10}\\/[a-z0-9])"
            )
        );
        CommentCell::loadFromComment(comment);
    }

    bool hideCommentFromText(const std::string_view settingKey, const std::regex& blocked) noexcept {
        if (!Mod::get()->getSettingValue<bool>(settingKey)) return false;

        std::smatch matched;
        if (std::regex_search(m_fields->m_commentString, matched, blocked)) {
            if (Mod::get()->getSettingValue<bool>("Log actions")) log::info(
                "hid comment id #{} because setting '{}' found '{}'",
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
            if (Mod::get()->getSettingValue<bool>("Log actions")) log::info(
                "hid comment id #{} because setting '{}' found that the commenter's userID is '{}'",
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