#include <cas/real-value.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Load RealValue and modify it", "[realvalue]") {
  cas::RealValue<unsigned int> rv = 5;
  REQUIRE(*rv == 5);
  rv = 10;
  REQUIRE(*rv == 5);
  cas::RealValueProvider::tickRegistry();
  REQUIRE(*rv == 10);
}
