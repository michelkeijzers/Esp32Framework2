#pragma once

#include <string>
#include <vector>

/**
 * Utility class for extracting parameters from HTTP URIs with wildcard patterns.
 * Uses simple string matching instead of regex (no exceptions needed).
 * 
 * Patterns use asterisk as wildcard separator.
 * Examples: /api/v1/presets/X, /api/v1/presets/X/channels/Y
 */
class UriParamExtractor {
public:
    /**
     * Extract all wildcard parameters from a URI given a pattern.
     * @param pattern The URI pattern with wildcards where X represents parameter
     * @param uri The actual request URI
     * @return Vector of extracted parameters, or empty if no match
     */
    static std::vector<std::string> extractParams(const std::string& pattern, const std::string& uri) {
        std::vector<std::string> params;
        
        // Split pattern by * to get literal parts
        std::vector<std::string> pattern_parts;
        size_t start = 0;
        size_t pos = pattern.find('*');
        
        // First part (before first *)
        pattern_parts.push_back(pattern.substr(0, pos));
        start = pos != std::string::npos ? pos + 1 : pattern.length();
        
        // Middle parts (between *)
        while (pos != std::string::npos) {
            pos = pattern.find('*', start);
            if (pos != std::string::npos) {
                pattern_parts.push_back(pattern.substr(start, pos - start));
                start = pos + 1;
            } else {
                pattern_parts.push_back(pattern.substr(start));
                break;
            }
        }
        
        // Now match URI against pattern parts
        size_t uri_pos = 0;
        
        for (size_t part_idx = 0; part_idx < pattern_parts.size(); ++part_idx) {
            const std::string& part = pattern_parts[part_idx];
            
            if (part_idx == 0) {
                // First part must match at start of URI
                if (uri.substr(0, part.length()) != part) {
                    return {};  // No match
                }
                uri_pos = part.length();
            } else if (part_idx == pattern_parts.size() - 1) {
                // Last part must match at end of URI
                if (part.empty()) {
                    // Pattern ended with *, so rest of URI is the last parameter
                    if (uri_pos < uri.length()) {
                        params.push_back(uri.substr(uri_pos));
                    }
                } else if (uri.length() >= uri_pos + part.length()) {
                    if (uri.substr(uri.length() - part.length()) == part) {
                        // Extract parameter between previous * and this literal
                        std::string param = uri.substr(uri_pos, uri.length() - part.length() - uri_pos);
                        params.push_back(param);
                    } else {
                        return {};  // No match
                    }
                } else {
                    return {};  // Not enough characters
                }
            } else {
                // Middle part - find it in URI and extract parameter before it
                size_t found = uri.find(part, uri_pos);
                if (found == std::string::npos) {
                    return {};  // Part not found
                }
                // Extract parameter between last position and this part
                std::string param = uri.substr(uri_pos, found - uri_pos);
                params.push_back(param);
                uri_pos = found + part.length();
            }
        }
        
        return params;
    }
    
    /**
     * Extract first parameter from URI (common case).
     * @param pattern The URI pattern with wildcard
     * @param uri The actual request URI
     * @return First parameter if found, empty string otherwise
     */
    static std::string extractFirstParam(const std::string& pattern, const std::string& uri) {
        auto params = extractParams(pattern, uri);
        return params.empty() ? "" : params[0];
    }
    
    /**
     * Try to convert first parameter to uint32_t.
     * @param pattern The URI pattern with wildcard
     * @param uri The actual request URI
     * @return Parameter as uint32_t if found and valid, 0 otherwise
     */
    static uint32_t extractFirstParamAsUint32(const std::string& pattern, const std::string& uri) {
        auto param = extractFirstParam(pattern, uri);
        if (param.empty()) return 0;
        
        uint32_t result = 0;
        for (char c : param) {
            if (c < '0' || c > '9') {
                return 0;  // Invalid character, not a number
            }
            result = result * 10 + (c - '0');
        }
        return result;
    }
};
