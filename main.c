#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

struct s_g {
    int nbr_of_philos;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    int nb_of_eat;
    pthread_mutex_t *forks;
    int nb_done;
};

typedef struct g_philo
{
    int id;
    pthread_t profil;
    int is_done;
    long    last_time_eat;
    struct s_g * g;
    // create var last_time_eat[i] = 
} t_philo;




long time_now()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long time_mili = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
    return time_mili;
}
void *routine_philos(void *arg)
{
    int id;
    t_philo *philos = (t_philo*)arg;
    id = philos->id;
    int count_eat;

    count_eat = 0;
    while (philos->g->nb_of_eat == -1 || count_eat != philos->g->nb_of_eat)
    {
        pthread_mutex_lock(&philos->g->forks[id - 1]);
        printf("%lu %d take a fork L\n", time_now(), id);

        pthread_mutex_lock(&philos->g->forks[id % philos->g->nbr_of_philos]);
        printf("%lu %d take a fork R\n", time_now(), id);

        printf("%lu %d is eating\n", time_now(), id);
        philos->last_time_eat = time_now();
        count_eat++;
        usleep(philos->g->time_to_eat * 1000);

        pthread_mutex_unlock(&philos->g->forks[id - 1]);
        pthread_mutex_unlock(&philos->g->forks[id % philos->g->nbr_of_philos]);

        printf("%lu %d is sleeping\n", time_now(), id);
        usleep(philos->g->time_to_eat * 1000);

        printf("%lu %d is thinking\n", time_now(), id);
    }
    philos->is_done = 1;
    philos->g->nb_done++;
    return NULL;
}


struct s_g * get_data(int argc, char **argv)
{
    struct s_g *g;

    g = malloc(sizeof(struct s_g));

    g->nb_of_eat = -1;
    g->nb_done = 0;
    g->nbr_of_philos = atoi(argv[1]); 
    g->time_to_die = atoi(argv[2]); 
    g->time_to_eat = atoi(argv[3]); 
    g->time_to_sleep = atoi(argv[4]);
    if (argc == 6)
        g->nb_of_eat = atoi(argv[5]); 
   return g;
}


int main(int argc, char **argv)
{
    int philo_is_dead;

    t_philo *philos;
    struct s_g *g;

    g = get_data(argc, argv);
    philos = malloc(sizeof(t_philo) * g->nbr_of_philos);

    // philos->profil = malloc(sizeof(pthread_t) * g->nbr_of_philos);
    g->forks = malloc(sizeof(pthread_mutex_t) * g->nbr_of_philos);
    // philos->last_time_eat = malloc(sizeof(long) * g->nbr_of_philos);



    
    int i = 0;
    while (i < g->nbr_of_philos)
    {
        pthread_mutex_init(&g->forks[i], NULL);
        i++;
    }

    i = 0;
    while (i < g->nbr_of_philos)
    {
        philos[i].id = i + 1;
        philos[i].is_done = 0;
        philos[i].last_time_eat = time_now();
        philos[i].g = g;
        pthread_create(&philos[i].profil, NULL, routine_philos, &philos[i]);
        usleep(100);
        i++;
    }
    while (philos->g->nb_done < g->nbr_of_philos)
    {
        // check if phili is die
        // check_life = time now - last time eat 
        // if check_life more then > time to die arg  ===> philo is die
        //int philo_is_dead;
        i = 0;
        t_philo *philo;
        while(i < g->nbr_of_philos)
        {
            philo_is_dead = time_now() - philos[i].last_time_eat;
            if (philo_is_dead > g->time_to_die && !philos[i].is_done)
            {
                pthread_mutex_t test;
                pthread_mutex_init(&test, NULL);
                pthread_mutex_lock(&test);
                printf("%lu %d is dead\n", time_now(), i + 1);
                return(1);
            }
            i++;
        }
    }
    printf("%d %d\n", philos->g->nb_done, g->nbr_of_philos);


    // i = 0;
    // while (i < g->nbr_of_philos)
    // {
    //     pthread_join(philos[i]->profil[i], NULL);
    //     i++;
    // }

    return (0);
}
