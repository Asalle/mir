/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Kevin DuBois <kevin.dubois@canonical.com>
 */

#include "mock_buffer.h"

#include "mir/compositor/buffer_swapper_double.h"
#include "mir_test/multithread_harness.h"

#include <thread>

namespace mc = mir::compositor;
namespace mt = mir::testing;
namespace geom = mir::geometry;

const int num_iterations = 100;

void server_work(std::shared_ptr<mc::BufferSwapper> swapper ,
                 mt::Synchronizer<mc::Buffer*>* synchronizer,
                 int tid )
{
    mc::Buffer* buf;
    for (int i=0; i< num_iterations; i++)
    {

        buf = swapper->dequeue_free_buffer();
        synchronizer->set_thread_data(buf, tid);
        synchronizer->child_sync();

        swapper->queue_finished_buffer();
        synchronizer->child_sync();
    }
}

void client_work(std::shared_ptr<mc::BufferSwapper> swapper,
                 mt::Synchronizer<mc::Buffer*>* synchronizer,
                int tid )
{

    mc::Buffer* buf;
    for (int i=0; i< num_iterations; i++)
    {
        buf = swapper->grab_last_posted();
        synchronizer->set_thread_data(buf, tid);
        synchronizer->child_sync();

        swapper->ungrab();
        synchronizer->child_sync();

    }

}

TEST(buffer_swapper_double_stress, simple_swaps0)
{
    std::chrono::milliseconds timeout(5000);

    geom::Width w {1024};
    geom::Height h {768};
    geom::Stride s {1024};
    mc::PixelFormat pf {mc::PixelFormat::rgba_8888};

    std::unique_ptr<mc::Buffer> buffer_a(new mc::MockBuffer(w, h, s, pf));
    std::unique_ptr<mc::Buffer> buffer_b(new mc::MockBuffer(w, h, s, pf));

    auto swapper = std::make_shared<mc::BufferSwapperDouble>(
            std::move(buffer_a),
            std::move(buffer_b));

    /* use these condition variables to poke and control the two threads */
    mt::Synchronizer<mc::Buffer*> synchronizer(2);

    std::thread t1(mt::manager_thread<mc::BufferSwapper, mc::Buffer*>,
                   client_work, swapper, 0, &synchronizer, timeout);
    std::thread t2(mt::manager_thread<mc::BufferSwapper, mc::Buffer*>,
                   server_work, swapper, 1, &synchronizer, timeout);

    mc::Buffer* dequeued, *grabbed;
    for(int i=0; i< num_iterations; i++)
    {
        std::cout << "sync control" << i << std::endl;
        synchronizer.control_wait();

        dequeued = synchronizer.get_thread_data(0); 
        grabbed  = synchronizer.get_thread_data(1); 
        EXPECT_NE(dequeued, grabbed);

        synchronizer.control_activate();
 
        synchronizer.control_wait();
        synchronizer.control_activate();
    }

    t1.join();
    t2.join();
}




































std::mutex mut;
bool baddie = false;
const int num_it2 = 300;
void server_work0(std::shared_ptr<mc::BufferSwapper> swapper ,
                 mt::Synchronizer<mc::Buffer*>* synchronizer,
                 int tid )
{
    mc::Buffer* buf;
    for(;;)
    {
        buf = swapper->dequeue_free_buffer();
        swapper->queue_finished_buffer();
        synchronizer->set_thread_data(buf, tid);
        if(synchronizer->child_check_pause(tid)) break;
    }
}

void client_work0(std::shared_ptr<mc::BufferSwapper> swapper,
                 mt::Synchronizer<mc::Buffer*>* synchronizer,
                int tid )
{
    mc::Buffer* buf;
    for (;;)
    {
        for (int j=0; j< 100; j++)
        {
            buf = swapper->grab_last_posted();
            swapper->ungrab();
        }

        synchronizer->set_thread_data(buf, tid);
        if(synchronizer->child_check_pause(tid)) break;
    }

    std::cout << "DONE with primary\n";
}

TEST(buffer_swapper_double_timing, stress_swaps)
{
    std::chrono::milliseconds timeout(5000);

    geom::Width w {1024};
    geom::Height h {768};
    geom::Stride s {1024};
    mc::PixelFormat pf {mc::PixelFormat::rgba_8888};

    std::unique_ptr<mc::Buffer> buffer_a(new mc::MockBuffer(w, h, s, pf));
    std::unique_ptr<mc::Buffer> buffer_b(new mc::MockBuffer(w, h, s, pf));

    auto swapper = std::make_shared<mc::BufferSwapperDouble>(
            std::move(buffer_a),
            std::move(buffer_b));

    mt::Synchronizer<mc::Buffer*> synchronizer(2);

    std::thread t1(mt::manager_thread<mc::BufferSwapper, mc::Buffer*>,
                   client_work0, swapper, 0, &synchronizer, timeout);
    std::thread t2(mt::manager_thread<mc::BufferSwapper, mc::Buffer*>,
                   server_work0, swapper, 1, &synchronizer, timeout);

    mc::Buffer* dequeued, *grabbed;
    for(int i=0; i< num_it2; i++)
    {
        synchronizer.enforce_child_pause(1);
        synchronizer.enforce_child_pause(0);
        synchronizer.control_wait();

        std::cout << "DONE WAITING\t\t\t" << i << "\n";

        dequeued = synchronizer.get_thread_data(0); 
        grabbed  = synchronizer.get_thread_data(1); 
        EXPECT_EQ(dequeued, grabbed);

        synchronizer.control_activate(); 
    }

    /* kill all threads */
    synchronizer.enforce_child_pause(1);
    synchronizer.enforce_child_pause(0);
    synchronizer.control_wait();
    synchronizer.set_kill();
    synchronizer.control_activate(); 

    t1.join();
    t2.join();
}
