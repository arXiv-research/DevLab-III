In the previous README.md of Guild-ai you use Guild to run train.py — a sample training script. In this section, you run the same script with different hyperparameter values to find lower values of loss. This process is known as hyperparameter optimization, or hyperparameter tuning.

Guild supports hyperparameter optimization using different search methods:

1.  Manual search
2.  Grid Search
3.  Random search
4.  Bayesian optimization


Here’s the loss function used in train.py:

loss = (np.sin(5 * x) * (1 - np.tanh(x ** 2)) + np.random.randn() * noise)

The relationship between x and loss is plotted below. Optimal values for x are around -0.3.

bayesian-optimization

Plot of x on the horizontal axis to loss on the vertical axis (image by Gilles Louppe, Manoj Kumar)


In a real scenario, we don’t know optimal hyperparameter values. We need to search for them.


Manual Search
When experimenting with hyperparameters, it’s often useful to start with values based on prior experience or intuition.

Guild lets you run multiple trials in a batch by specifying them as a list in the form [VAL1,VAL2,…,VALN].

Run three trials of train.py using different values for x:

$
guild run train.py x='[-1,0,1]'
You are about to run train.py as a batch (3 trials)
  noise: 0.1
  x: [-1, 0, 1]
Continue? (Y/n)


IMPORTANT
The single quotes used for the argument [-1,0,1] are required if you’re using Z shell (zsh). This is the default shell used in macOS. If you’re using Bash or Bourne shells, you can omit them provided the list argument does not contain spaces.

Press Enter to start the batch.

Guild runs train.py three times, once for each specified value of x.

Show the runs with guild runs:

$
guild runs
[1:1933bdcb]  train.py   2020-01-14 09:38:15  completed  noise=0.1 x=1
[2:83dc048d]  train.py   2020-01-14 09:38:14  completed  noise=0.1 x=0
[3:468bb240]  train.py   2020-01-14 09:38:14  completed  noise=0.1 x=-1
[4:bfcff413]  train.py+  2020-01-14 09:38:13  completed
[5:68f4da74]  train.py   2020-01-14 08:42:54  completed  noise=0.1 x=0.1

Runs 1 through 3 are trial runs generated by the command. Run 4 is a batch run. Batch runs generate trials and are denoted using + in their name.


Compare Runs
Compare loss across runs using guild compare:

$
guild compare --min loss
Guild starts an interactive console application that lets you browse experiment results. Runs with lower loss appear at the top because --min loss is specified in the command. Use arrow keys to navigate. Press 1 to sort by the current column in ascending order or 2 to sort in descending order. Press ? for a list of supported commands.

compare-start
Compare experiment results — press ? for a list of commands; press q to exit

Press q to exit Guild Compare.

HIGHLIGHT
Guild Compare is a convenient way to review run results. It works across remote terminal connections as well as local environments and doesn’t require a web browser or port configuration.

Next, run four trials — one for for each unique combination of flag values:

$
guild run train.py x=[-0.5,0.5] noise=[0.1,0.2]
You are about to run train.py as a batch (4 trials)
  noise: [0.1, 0.2]
  x: [-0.5, 0.5]
Continue? (Y/n)
Press Enter to start the batch.

Show the top-3 best runs:

$
guild compare --table --min loss --top 3
The --table option tells Guild to show results without running in interactive mode. The --top option tells Guild to show only the top-N runs based on the sort order.

Runs where x is -0.5 should have the lowest values for loss. This is consistent with our expectation from the plot above.


View Runs in TensorBoard
Guild integrates TensorBoard for exploring runs. Use guild tensorboard to view current runs:

$
guild tensorboard
Guild starts TensorBoard and opens a new tab in your browser.

Select the HParams tab and then select Parallel Coordinates View.

tb-hparams
  Compare runs using Parallel Coordinates View

The Parallel Coordinates View highlights runs that perform better along various axes. Click-and-drag along the loss axis to highlight runs with the lowest values. Note that runs where x is -0.5 are highlighted.

HIGHLIGHT
Guild automatically generates HParam summaries from Guild runs to simply the process of comparing runs in TensorBoard.

Return to the command terminal and press Ctrl-C to stop TensorBoard.


Grid Search
Grid search is a systematic search across a subset of hyperparameter space. Guild supports a special sequence function syntax for specifying value ranges. Ranges are specified using the format FUNCTION[START:END:STEP_OR_COUNT] where FUNCTION is the type of sequence and START and END mark the start and end of the sequence respectively. STEP_OR_COUNT is the range step or value count depending on the function used.

Use the linspace flag function to run four trials where x is evenly spaced between -0.6 and 0.6:

$
guild run train.py x=linspace[-0.6:0.6:4]
You are about to run train.py as a batch (max 20 trials, minimize loss)
  noise: 0.1
  x: [-0.6, -0.2, 0.2, 0.6]
Continue? (Y/n)

Guild expands the function linspace[-0.6:0.6:4] to the list [-0.6, -0.2, 0.2, 0.6].

Press Enter to start the batch.

Guild runs trials for each value of x generated by the linspace function.

TIP
Use sequence functions for multiple flags to expand a grid search to the Cartesian product of the flag value sets.


Random Search
Guild supports random search over a both uniform and log-uniform distributions.

Search space is specified by special search space functions, which include uniform and loguniform. The uniform function name may be omitted.

To search over a uniformly distributed range of values, specify a flag value in the format [MIN:MAX]. By default, Guild runs 20 trials using randomly chosen values within the specified range. Use --max-trials to specify a different limit.

Start a random search over x with 5 trials:

$
guild run train.py x=[-2.0:2.0] --max-trials 5
You are about to run train.py with random search (max 5 trials)
  noise: 0.1
  x: [-2.0:2.0]
Continue? (Y/n)

Press Enter to start the batch.

Guild runs train.py five times using randomly sampled values for x from -2.0 to 2.0.


Bayesian Optimization
Bayesian optimization uses light-weight probabilistic models to suggest hyperparameter values believed to improve a future result based on previous results.

By default, Guild attempts to minimize loss as an objective. The train.py script logs loss as a scalar. If the script uses a different objective, specify the scalar using --minimize or --maximize options with guild run.

Run 10 trials using the gp optimizer, which uses Bayesian optimization with Gaussian processes, and search over values of x between -2.0 and 2.0:

$
guild run train.py x=[-2.0:2.0] --optimizer gp --max-trials 10
You are about to run train.py with 'skopt:gp' optimizer (max 10 trials, minimize loss)
  noise: 0.1
  x: [-2.0:2.0]
Optimizer flags:
  acq-func: gp_hedge
  kappa: 1.96
  noise: gaussian
  random-starts: 3
  xi: 0.05
Continue? (Y/n)

Press Enter to start the batch.

The gp optimizer seeds the batch with three random starts. After the third random start, the optimizer uses previous results to suggest candidates for x between -2.0 and 2.0 to minimize loss.

For list of available optimizes, see Optimizers Reference.


Restart Batch to Continue Optimization
Guild optimizers use previous results from trials of the same batch. If you want to continue a search for hyperparameters using previous results, restart the batch run.

To restart a run, specify --restart with guild run.

To get the run ID of the gp batch, run:

$
guild select -Fo train.py+gp
Guild prints the ID of the last gp batch run. Copy this value for the next command.

Restart the batch to generate another ten trials, replacing BATCH_RUN_ID below with ID from the previous command:

$
guild run -Fo random-starts=0 --restart BATCH_RUN_ID
You are about to start ... (train.py) with 'skopt:gp' optimizer (max 10 trials)
  noise: 0.1
  x: [-2.0:2.0]
Optimizer flags:
  acq-func: gp_hedge
  kappa: 1.96
  noise: gaussian
  random-starts: 0
  xi: 0.05
Continue? (Y/n)

TIP
You can type the first few characters of the batch run ID rather than use the full ID if those characters uniquely identify the run.

TIP
If you’re running Linux, macOS, or another POSIX environment, you can use command substitution and guild select to specify a run ID argument. For example, guild run --restart $(guild select -Fo train.py+gp) restarts the latest train.py+gp.

Press Enter to restart the batch.

Guild generates another ten trials for the batch. Guild uses the previous trials generated earlier as inputs to the optimization.

The option -Fo random-starts=0 is an optimizer flag that sets the number of random starts to 0.


Evaluate Bayesian Optimization Results
Run guild compare for the last 20 runs to evaluate the results of the Bayesian optimization:

$
guild compare :20
By default, Guild orders runs by start time with most recent runs listed first. Note values for x suggested by the gp optimizer. If the optimizer succeeds in finding values for x that minimize loss, you should see more values around -0.3 toward the top of the list. These are the most recent runs and benefit from more trial data.

Press q to exit Guild Compare.

You can also use TensorBoard to evaluate optimization results.

Start TensorBoard for the last 20 runs:

$
guild tensorboard :20
Guild starts TensorBoard and opens a new browser tab.

Select the HParams tag and then select Scatter Plot Matrix View. TensorBoard shows scatter plots of hyperparameters and metrics.

In the left side panel, deselect the following hyperparameters:

noise
sourcecode
In the same panel, deselect the following metrics:

time
TensorBoard displays a plot of loss against x. Each point on the plot represents a trial. Given the known relationship between loss and x (see plot above), the Bayesian optimizer should spend more time exploring values for x around -0.3. This will appear as a cluster of trials along the bottom of the plot between -0.4 and -0.2.

tb-hparams-scatter
Plot loss against x to evaluate the Bayesian optimization results. Effective optimization spends more time exploring values of x around -0.3, shown as a cluster of trials along the bottom axis.

Return to the command terminal and press Ctrl-C to stop TensorBoard.

Summary
In this section, you use various techniques to run train.py.

HIGHLIGHTS

Guild supports a variety of optimization methods including grid search, random search, and Bayesian optimization.
Use Guild Compare and TensorBoard to study optimization trials to help find the best hyperparameters for your models.