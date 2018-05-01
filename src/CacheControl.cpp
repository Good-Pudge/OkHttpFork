//
// Created by Good_Pudge.
//

#include <sstream>
#include <ohf/CacheControl.hpp>
#include <ohf/Exception.hpp>
#include "util/util.hpp"
#include "util/string.hpp"

using namespace util;
using namespace util::string;

namespace ohf {
    CacheControl::CacheControl(const Headers &headers) : CacheControl() {
        std::vector<std::string> cache_control = split(headers.get("cache-control"),
                                                       std::vector<std::string>{", ", ","});

        for (auto &element : cache_control) {
            util::string::toLower(element);
            std::vector<std::string> keyValue = split(element, "=");
            if (keyValue.size() == 1) {
                std::string key = keyValue[0];
                if (key == "public")
                    mPublic = true;
                else if (key == "private")
                    mPrivate = true;
                else if (key == "no-cache")
                    mNoCache = true;
                else if (key == "only-if-cached")
                    mOnlyIfCached = true;
                else if (key == "must-revalidate")
                    mMustRevalidate = true;
                else if (key == "proxy-revalidate" && mPublic)
                    mMustRevalidate = true;
                else if (key == "immutable")
                    mImmutable = true;
                else if (key == "no-store")
                    mNoStore = true;
                else if (key == "no-transform")
                    mNoTransform = true;
            } else if (keyValue.size() == 2) {
                std::string key = keyValue[0];
                std::string value = keyValue[1];
                if (key == "max-age")
                    try {
                        mMaxAge = TimeUnit::seconds(std::stoi(value));
                    } catch (const std::invalid_argument&) {
                        throw Exception(Exception::Code::INVALID_MAX_AGE, "Invalid Max-Age: " + value);
                    }
                else if (key == "s-maxage" && mPublic)
                    try {
                        mSMaxAge = TimeUnit::seconds(std::stoi(value));
                    } catch (const std::invalid_argument&) {
                        throw Exception(Exception::Code::INVALID_S_MAX_AGE, "Invalid S-MaxAge: " + value);
                    }
                else if (key == "max-stale")
                    try {
                        mMaxStale = TimeUnit::seconds(std::stoi(value));
                    } catch (const std::invalid_argument&) {
                        throw Exception(Exception::Code::INVALID_MAX_STALE, "Invalid Max-Stale: " + value);
                    }
                else if (key == "min-fresh")
                    try {
                        mMinFresh = TimeUnit::seconds(std::stoi(value));
                    } catch (const std::invalid_argument&) {
                        throw Exception(Exception::Code::INVALID_MIN_FRESH, "Invalid Min-Fresh: " + value);
                    }
                // stale-while-revalidate=<seconds> ?
                // stale-if-error=<seconds> ?
            }
        }
    }

    bool CacheControl::isPublic() const {
        return mPublic;
    }

    bool CacheControl::isPrivate() const {
        return mPrivate;
    }

    bool CacheControl::immutable() const {
        return mImmutable;
    }

    bool CacheControl::mustRevalidate() const {
        return mMustRevalidate;
    }

    bool CacheControl::noCache() const {
        return mNoCache;
    }

    bool CacheControl::noStore() const {
        return mNoStore;
    }

    bool CacheControl::noTransform() const {
        return mNoTransform;
    }

    bool CacheControl::onlyIfCached() const {
        return mOnlyIfCached;
    }

    TimeUnit CacheControl::maxAge() const {
        return mMaxAge;
    }

    TimeUnit CacheControl::maxStale() const {
        return mMaxStale;
    }

    TimeUnit CacheControl::minFresh() const {
        return mMinFresh;
    }

    TimeUnit CacheControl::sMaxAge() const {
        return mSMaxAge;
    }

    CacheControl::CacheControl() :
            mPublic(false),
            mPrivate(false),
            mNoCache(false),
            mOnlyIfCached(false),
            mMustRevalidate(false),
            mImmutable(false),
            mNoStore(false),
            mNoTransform(false),
            mMaxAge(TimeUnit::MINUS_ONE_SECOND),
            mSMaxAge(TimeUnit::MINUS_ONE_SECOND),
            mMaxStale(TimeUnit::MINUS_ONE_SECOND),
            mMinFresh(TimeUnit::MINUS_ONE_SECOND)
    {}

    CacheControl::CacheControl(const Builder *builder) :
            // bool
            mPublic(false),
            mPrivate(false),
            mMustRevalidate(false),
            mNoCache(builder->mNoCache),
            mOnlyIfCached(builder->mOnlyIfCached),
            mImmutable(builder->mImmutable),
            mNoStore(builder->mNoStore),
            mNoTransform(builder->mNoTransform),
            // TimeUnit
            mSMaxAge(TimeUnit::MINUS_ONE_SECOND),
            mMaxAge(builder->mMaxAge),
            mMaxStale(builder->mMaxStale),
            mMinFresh(builder->mMinFresh)
    {}

    std::string CacheControl::toString() const {
        std::stringstream ss;
        if (mPublic) ss << "public, ";
        if (mPrivate) ss << "private, ";
        if (mNoCache) ss << "no-cache, ";
        if (mOnlyIfCached) ss << "only-if-cached, ";
        if (mMustRevalidate) ss << "must-revalidate, ";
        if (mImmutable) ss << "immutable, ";
        if (mNoStore) ss << "no-store, ";
        if (mNoTransform) ss << "no-transform, ";
        if (mMaxAge > TimeUnit::MINUS_ONE_SECOND) ss << "max-age=" << mMaxAge.std_time() << ", ";
        if (mSMaxAge > TimeUnit::MINUS_ONE_SECOND) ss << "s-maxage" << mSMaxAge.std_time() << ", ";
        if (mMaxStale > TimeUnit::MINUS_ONE_SECOND) ss << "max-stale=" << mMaxStale.std_time() << ", ";
        if (mMinFresh > TimeUnit::MINUS_ONE_SECOND) ss << "min-fresh=" << mMinFresh.std_time() << ", ";
        std::string str = ss.str();
        if (!str.empty()) str.erase(str.length() - 2, 2);
        return str;
    }

    bool CacheControl::operator ==(const CacheControl &cc) const {
        return
                // bool
                this->mPublic == cc.mPublic
                && this->mPrivate == cc.mPrivate
                && this->mNoCache == cc.mNoCache
                && this->mOnlyIfCached == cc.mOnlyIfCached
                && this->mMustRevalidate == cc.mMustRevalidate
                && this->mImmutable == cc.mImmutable
                && this->mNoStore == cc.mNoStore
                && this->mNoTransform == cc.mNoTransform
                // TimeUnit
                && this->mMaxAge == cc.mMaxAge
                && this->mSMaxAge == cc.mSMaxAge
                && this->mMaxStale == cc.mMaxStale
                && this->mMinFresh == cc.mMinFresh;
    }

    std::ostream &operator <<(std::ostream &stream, const CacheControl &cc) {
        return stream << cc.toString();
    }
}