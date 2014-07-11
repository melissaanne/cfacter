/**
 * @file
 * Declares the fact collection.
 */
#ifndef FACTER_FACTS_COLLECTION_HPP_
#define FACTER_FACTS_COLLECTION_HPP_

#include "resolver.hpp"
#include "value.hpp"
#include "external/resolver.hpp"
#include <list>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>
#include <iostream>

namespace facter { namespace facts {

    /**
     * The supported output format for the fact collection.
     */
    enum class format
    {
        /**
         * Use ruby "hash" as the format (default).
         */
        hash,
        /**
         * Use JSON as the format.
         */
        json,
        /**
         * Use YAML as the format.
         */
        yaml
    };

    /**
     * Represents the fact collection.
     * The fact collection is responsible for resolving and storing facts.
     */
    struct collection
    {
        /**
         * Constructs a fact collection.
         */
        collection();

        /**
         * Destructor for fact collection.
         */
        ~collection();

        /**
         * Prevents the fact collection from being copied.
         */
        collection(collection const&) = delete;
        /**
         * Prevents the fact collection from being copied.
         * @returns Returns this fact collection.
         */
        collection& operator=(collection const&) = delete;
        /**
         * Moves the given fact collection into this fact collection.
         * @param other The fact collection to move into this fact collection.
         */
        collection(collection&& other) = default;
        /**
         * Moves the given fact collection into this fact collection.
         * @param other The fact collection to move into this fact collection.
         * @return Returns this fact collection.
         */
        collection& operator=(collection&& other) = default;

        /**
         * Adds the default facts to the collection.
         */
        void add_default_facts();

        /**
         * Adds a resolver to the fact collection.
         * The last resolver that was added for a particular name or pattern will "win" resolution.
         * @param res The resolver to add to the fact collection.
         */
        void add(std::shared_ptr<resolver> const& res);

        /**
         * Adds a fact to the fact collection.
         * @param name The name of the fact.
         * @param value The value of the fact.
         */
        void add(std::string&& name, std::unique_ptr<value>&& value);

        /**
         * Adds external facts to the fact collection.
         * If external facts are present, all facts will be resolved prior to adding the external facts.
         * @param directories The directories to search for external facts.  If empty, the default search paths will be used.
         */
        void add_external_facts(std::vector<std::string> const& directories = {});

        /**
         * Adds custom (Ruby) facts to the fact collection.
         * If custom facts are present, all facts will be resolved prior to adding the custom facts.
         * @param directories The directories to search for custom facts.  If empty, the default search paths will be used.
         */
        void add_custom_facts(std::vector<std::string> const& directories);

        /**
         * Removes a resolver from the fact collection.
         * @param res The resolver to remove from the fact collection.
         */
        void remove(std::shared_ptr<resolver> const& res);

        /**
         * Removes a fact by name.
         * @param name The name of the fact to remove.
         */
        void remove(std::string const& name);

        /**
         * Clears the entire fact collection.
         * This will remove all built-in facts and resolvers from the fact collection.
         */
        void clear();

        /**
         * Checks to see if the fact collection is empty.
         * All facts will be resolved to determine if the collection is empty.
         * @return Returns true if the fact collection is empty or false if it is not.
         */
        bool empty();

        /**
         * Gets the count of facts in the fact collection.
         * All facts will be resolved to determine the size of the collection.
         * @return Returns the number of facts in the fact collection.
         */
        size_t size();

        /**
         * Filters the collection to contain only facts with the given names.
         * @param names The names of the facts to filter the collection by.
         * @param add True if an empty string fact should be added for any missing facts in the set or false if not.
         */
        void filter(std::set<std::string> names, bool add = true);

        /**
         * Gets a fact value by name.
         * @tparam T The expected type of the value.
         * @param name The name of the fact to get the value of.
         * @param resolve True if resolution should take place or false if not.
         * @return Returns a pointer to the fact value or nullptr if the fact is not in the fact collection or the value is not the expected type.
         */
        template <typename T>
        T const* get(std::string const& name, bool resolve = true)
        {
            return dynamic_cast<T const*>(get_value(name, resolve));
        }

        /**
         * Gets a fact value by name
         * @param name The name of the fact to get the value of.
         * @return Returns a pointer to the fact value or nullptr if the fact is not in the fact collection.
         */
        value const* operator[](std::string const& name);

        /**
         * Enumerates all facts in the collection.
         * All facts will be resolved prior to enumeration.
         * @param func The callback function called for each fact in the collection.
         */
        void each(std::function<bool(std::string const&, value const*)> func);

        /**
         * Writes the contents of the fact collection to the given stream.
         * All facts will be resolved prior to writing.
         * @param stream The stream to write the facts to.
         * @param fmt The output format to use.
         * @return Returns the stream being written to.
         */
        std::ostream& write(std::ostream& stream, format fmt = format::hash);

     private:
        void resolve_facts();
        void resolve_fact(std::string const& name);
        value const* get_value(std::string const& name, bool resolve);
        void write_hash(std::ostream& stream) const;
        void write_json(std::ostream& stream) const;
        void write_yaml(std::ostream& stream) const;

        // Platform specific members
        void add_platform_facts();
        std::vector<std::string> get_external_fact_directories();
        std::vector<std::unique_ptr<external::resolver>> get_external_resolvers();

        std::map<std::string, std::unique_ptr<value>> _facts;
        std::list<std::shared_ptr<resolver>> _resolvers;
        std::multimap<std::string, std::shared_ptr<resolver>> _resolver_map;
        std::list<std::shared_ptr<resolver>> _pattern_resolvers;
    };

}}  // namespace facter::facts

#endif  // FACTER_FACTS_COLLECTION_HPP_
