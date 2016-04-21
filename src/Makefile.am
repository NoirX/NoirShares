DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

NOIRSHARES_CONFIG_INCLUDES=-I$(builddir)/config
NOIRSHARES_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

NOIRSHARES_INCLUDES += -I$(srcdir)/secp256k1/include
NOIRSHARES_INCLUDES += $(UNIVALUE_CFLAGS)

LIBNOIRSHARES_SERVER=libnoirshares_server.a
LIBNOIRSHARES_WALLET=libnoirshares_wallet.a
LIBNOIRSHARES_COMMON=libnoirshares_common.a
LIBNOIRSHARES_CONSENSUS=libnoirshares_consensus.a
LIBNOIRSHARES_CLI=libnoirshares_cli.a
LIBNOIRSHARES_UTIL=libnoirshares_util.a
LIBNOIRSHARES_CRYPTO=crypto/libnoirshares_crypto.a
LIBNOIRSHARESQT=qt/libnoirsharesqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libnoirshares_crypto.a \
  libnoirshares_util.a \
  libnoirshares_common.a \
  libnoirshares_consensus.a \
  libnoirshares_server.a \
  libnoirshares_cli.a
if ENABLE_WALLET
NOIRSHARES_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libnoirshares_wallet.a
endif
if ENABLE_ZMQ
EXTRA_LIBRARIES += libnoirshares_zmq.a
endif

if BUILD_NOIRSHARES_LIBS
lib_LTLIBRARIES = libnoirsharesconsensus.la
LIBNOIRSHARESCONSENSUS=libnoirsharesconsensus.la
else
LIBNOIRSHARESCONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_NOIRSHARESD
  bin_PROGRAMS += noirsharesd
endif

if BUILD_NOIRSHARES_UTILS
  bin_PROGRAMS += noirshares-cli noirshares-tx
endif

.PHONY: FORCE check-symbols check-security
# noirshares core #
NOIRSHARES_CORE_H = \
  addrman.h \
  base58.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  core_io.h \
  core_memusage.h \
  httprpc.h \
  httpserver.h \
  init.h \
  key.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  main.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  netbase.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  protocol.h \
  random.h \
  reverselock.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/register.h \
  scheduler.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/ismine.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  versionbits.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libnoirshares_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between noirsharesd and noirshares-qt
libnoirshares_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libnoirshares_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_server_a_SOURCES = \
  addrman.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  dbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(NOIRSHARES_CORE_H)

if ENABLE_ZMQ
LIBNOIRSHARES_ZMQ=libnoirshares_zmq.a

libnoirshares_zmq_a_CPPFLAGS = $(NOIRSHARES_INCLUDES) $(ZMQ_CFLAGS)
libnoirshares_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between noirsharesd and noirshares-qt, but only linked
# when wallet enabled
libnoirshares_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
libnoirshares_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_wallet_a_SOURCES = \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(NOIRSHARES_CORE_H)

# crypto primitives library
crypto_libnoirshares_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_CONFIG_INCLUDES)
crypto_libnoirshares_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libnoirshares_crypto_a_SOURCES = \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

# consensus: shared between all executables that validate any consensus rules.
libnoirshares_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
libnoirshares_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/noirsharesconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between noirsharesd, and noirshares-qt and non-server tools
libnoirshares_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
libnoirshares_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_common_a_SOURCES = \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/sign.cpp \
  script/standard.cpp \
  $(NOIRSHARES_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libnoirshares_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
libnoirshares_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_util_a_SOURCES = \
  support/pagelocker.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(NOIRSHARES_CORE_H)

if GLIBC_BACK_COMPAT
libnoirshares_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between noirshares-cli and noirshares-qt
libnoirshares_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
libnoirshares_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libnoirshares_cli_a_SOURCES = \
  rpc/client.cpp \
  $(NOIRSHARES_CORE_H)

nodist_libnoirshares_util_a_SOURCES = $(srcdir)/obj/build.h
#

# noirsharesd binary #
noirsharesd_SOURCES = noirsharesd.cpp
noirsharesd_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
noirsharesd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
noirsharesd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
noirsharesd_SOURCES += noirsharesd-res.rc
endif

noirsharesd_LDADD = \
  $(LIBNOIRSHARES_SERVER) \
  $(LIBNOIRSHARES_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBNOIRSHARES_UTIL) \
  $(LIBNOIRSHARES_CONSENSUS) \
  $(LIBNOIRSHARES_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_ZMQ
noirsharesd_LDADD += $(LIBNOIRSHARES_ZMQ) $(ZMQ_LIBS)
endif

if ENABLE_WALLET
noirsharesd_LDADD += libnoirshares_wallet.a
endif

noirsharesd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# noirshares-cli binary #
noirshares_cli_SOURCES = noirshares-cli.cpp
noirshares_cli_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES) $(EVENT_CFLAGS)
noirshares_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
noirshares_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
noirshares_cli_SOURCES += noirshares-cli-res.rc
endif

noirshares_cli_LDADD = \
  $(LIBNOIRSHARES_CLI) \
  $(LIBUNIVALUE) \
  $(LIBNOIRSHARES_UTIL)

noirshares_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# noirshares-tx binary #
noirshares_tx_SOURCES = noirshares-tx.cpp
noirshares_tx_CPPFLAGS = $(AM_CPPFLAGS) $(NOIRSHARES_INCLUDES)
noirshares_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
noirshares_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
noirshares_tx_SOURCES += noirshares-tx-res.rc
endif

noirshares_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBNOIRSHARES_COMMON) \
  $(LIBNOIRSHARES_UTIL) \
  $(LIBNOIRSHARES_CONSENSUS) \
  $(LIBNOIRSHARES_CRYPTO) \
  $(LIBSECP256K1)

noirshares_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# noirsharesconsensus library #
if BUILD_NOIRSHARES_LIBS
include_HEADERS = script/noirsharesconsensus.h
libnoirsharesconsensus_la_SOURCES = $(crypto_libnoirshares_crypto_a_SOURCES) $(libnoirshares_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libnoirsharesconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libnoirsharesconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libnoirsharesconsensus_la_LIBADD = $(LIBSECP256K1)
libnoirsharesconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_NOIRSHARES_INTERNAL
libnoirsharesconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += $(EXTRA_LIBRARIES)
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
