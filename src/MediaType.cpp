//
// Created by Good_Pudge.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <ohf/MediaType.hpp>
#include <ohf/Exception.hpp>
#include <ohf/Config.hpp>
#include "util/string.hpp"

namespace ohf {
    MediaType::MediaType(const std::string &str) {
        std::vector<std::string> values = util::string::split(str, "; ");
        if (values.empty())
            throw Exception(Exception::Code::INVALID_CONTENT_TYPE_LINE, "Invalid Content-Type line: " + str);
        // type / subtype
        std::vector<std::string> typeSubtype = util::string::split(values[0], "/");
        if (typeSubtype.size() != 2)
            throw Exception(Exception::Code::INVALID_MIME_TYPE, "Invalid MIME type: " + values[0]);
        mType = typeSubtype[0];
        mSubType = typeSubtype[1];
        // boundary / charset
        for (Uint32 i = 1; i < values.size(); i++) {
            std::string value = values[i];
            if (util::string::startsWith(value, "charset=")) {
                std::string charset = value.substr(8, value.length());
                util::string::toLower(charset);
                mCharset = charset;
            } else if (util::string::startsWith(value, "boundary="))
                mBoundary = value.substr(9, value.length());
        }
    }

    MediaType::MediaType(const char *str) : MediaType(std::string(str)) {
    }

    std::string MediaType::boundary() const {
        return mBoundary;
    }

    std::string MediaType::boundary(const std::string &defaultValue) const {
        return mBoundary.empty() ? defaultValue : mBoundary;
    }

    std::string MediaType::charset() const {
        return mCharset;
    }

    std::string MediaType::charset(const std::string &defaultValue) const {
        return mCharset.empty() ? defaultValue : mCharset;
    }

    std::string MediaType::subtype() const {
        return mSubType;
    }

    std::string MediaType::type() const {
        return mType;
    }

    bool MediaType::operator==(const MediaType &mediaType) const {
        return mediaType.mBoundary == this->mBoundary
               && mediaType.mCharset == this->mCharset
               && mediaType.mType == this->mType
               && mediaType.mSubType == this->mSubType;
    }

    std::string MediaType::toString() const {
        std::stringstream ss;

        std::string type = mType;
        std::string subtype = mSubType;
        if (!type.empty() && !subtype.empty())
            ss << type << '/' << subtype << "; ";

        std::string charset = mCharset;
        if (!charset.empty())
            ss << "charset=" << charset << "; ";

        std::string boundary = mBoundary;
        if (!boundary.empty())
            ss << "boundary=" << boundary << "; ";

        std::string str = ss.str();
        if (!str.empty())
            str.erase(str.length() - 2, 2);
        return str;
    }

    std::ostream &operator<<(std::ostream &stream, const MediaType &mediaType) {
        return stream << mediaType.toString();
    }
}