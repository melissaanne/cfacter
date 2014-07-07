/**
 * @file
 * Declares the OSX virtualization fact resolver.
 */
#ifndef FACTER_FACTS_OSX_VIRTUALIZATION_RESOLVER_HPP_
#define FACTER_FACTS_OSX_VIRTUALIZATION_RESOLVER_HPP_

#include "../posix/virtualization_resolver.hpp"

namespace facter { namespace facts { namespace osx {

    /**
     * Responsible for resolving virtualization facts.
     */
    struct virtualization_resolver : posix::virtualization_resolver
    {
     protected:
        /**
         * Gets the name of the hypervisor.
         * @param facts The fact map that is resolving facts.
         * @return Returns the name of the hypervisor or empty string if no hypervisor.
         */
        virtual std::string get_hypervisor(fact_map& facts);
    };

}}}  // namespace facter::facts::osx

#endif  // FACTER_FACTS_OSX_VIRTUALIZATION_RESOLVER_HPP_
