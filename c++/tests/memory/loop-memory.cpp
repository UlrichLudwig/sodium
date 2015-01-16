#include <sodium/sodium.h>
#include <unistd.h>
#include <stdio.h>

using namespace sodium;
using namespace std;

behavior<int> mkCount(event_sink<unit> eTick)
{
    transaction<> trans;
    event_loop<int> eCount;
    behavior<int> count = eCount.hold(0);
    event<int> eCount2 = eTick.snapshot<int,int>(count, [] (const unit&, const int& i) { return i+1; });
    eCount.loop(eCount2);
    return count;
}

/*!
 * Run:
 *     valgrind --tool=massif memory/loop-memory
 *     massif-visualizer massif.out.*
 *
 * What you should see:
 *
 *     flat memory usage profile (no leak)
 */
int main(int argc, char* argv[])
{
    event_sink<unit> eTick;
    //event<unit> eTick2 = eTick.map<unit>([] (const unit& u) { return u; });
    behavior<int> ticks0 = mkCount(eTick);
    
    #if 0
    behavior_sink<behavior<int>> tickss(ticks0);
    auto kill = switch_b<int>(tickss).updates().listen([] (const int& i) { printf("%d\n", i); });
    while (true) {
        eTick.send(unit());
        eTick.send(unit());
        eTick.send(unit());
        eTick.send(unit());
        tickss.send(mkCount(eTick));
        tickss.send(eTick.accum<int>(0, [] (const unit&, const int& i) -> int { return i+1; }));
    }
    kill();
    #endif
    return 0;
}

