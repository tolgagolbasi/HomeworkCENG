function [] = createFit3()
    fig1 = open('CFRAC.fig');
    fig2 = open('Pollard.fig');
    ax1 = get(fig1, 'Children');
    ax2 = get(fig2, 'Children');
    for i = 1 : numel(ax2) 
       ax2Children = get(ax2(i),'Children');
       copyobj(ax2Children, ax1(i));
    end
end